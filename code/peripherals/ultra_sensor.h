#ifndef ULTRA_SENSOR_H
#define ULTRA_SENSOR_H

#define F_CPU 8000000UL		//!< clock speed of Atmega328p - 8MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "drive_motor.h"
#include "../serial/uart.h"

/**
* @defgroup ledIndic LED Wall Collision Warning
* @brief Defines the use of the LED wall collision warning system
* @{
*/
#define LED_PORT		PORTB0						//!< LED port (PORTB0)
#define LED_ON			PORTB |= (1<<LED_PORT)		//!< turn on LED (PORTB0)
#define LED_OFF			PORTB &= ~(1<<LED_PORT)		//!< turn off LED (PORTB0)
/**@}*/

/**
* @defgroup ultraSenFunct Ultrasonic Distance Sensor
* @brief Allows the wall collision functionality of robot
* @{
*/

#define TRIG_PORT		PORTD7	//!< trigger port (PORTD7)
#define ECHO_PORT		PORTD2	//!< echo echo echo...echo port (PORTD2)

static volatile unsigned int	pulse; 			//!< pulse from ultrasonic sensor
static volatile unsigned int	i;				//!< state decider for timer 1
static volatile unsigned int	distance;		//!< actual distance calculated

int stop_dist = 25;								//!< boarder distance for backup

//! Initialize ultrasonic sensor pins
void init_ultrasensor(){
	pulse = 0;
	i = 0;
	
	DDRD |= (1<<TRIG_PORT);		// trigger output
	DDRD &= ~(1<<ECHO_PORT);	// echo input
	DDRB |= (1<<LED_PORT);		// LED indicator
	
	EIMSK |= (1<<INT0);			// enable interrupt zero
	EICRA |= (1<<ISC00);		// positive edge
	TCCR1A = 0;					// timer for distance t1
	
	sei();						// enable global interrupts
}

//! Detects wall collision & brakes accordingly
void ultra_controls(){ // controls everything on bot
	PORTD |= (1<<7);
	_delay_us(15);
	PORTD &= ~(1<<7);
	distance = pulse/58;
	_delay_ms(20);
	
	if(distance < stop_dist){
		drive_motor(-100, -100);
		LED_ON;
	}
	else{
		LED_OFF;
		readString();
		if(done)
			drive_motor(l, r);
		else
			drive_motor(0, 0);		
	}
}

//! Internal Interrupt 0 ISR
ISR(INT0_vect){
	if(i == 1){
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}
	if(i == 0){
		TCCR1B |= (1<<CS11);
		i = 1;
	}
	distance = pulse/58;
	if(distance < stop_dist){
		drive_motor(-100, -100);
		LED_ON;
	}
	else{
		drive_motor(l, r);
		LED_OFF;
	}
}
/**@}*/
#endif