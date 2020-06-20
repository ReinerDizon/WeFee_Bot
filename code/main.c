/**
*	@mainpage
* Authors: Reiner Dizon & Angel Solis
* \n Class: CPE 301L
* \n Section: 1001
* \n Final Project
* \n INCOMPLETE
*/

#define F_CPU 8000000UL		//!< clock speed of Atmega328p - 8MHz
#define BUTTON (PINB & (1<<PINB4)) // check for button press

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include "peripherals/drive_motor.h"
#include "peripherals/ultra_sensor.h"
#include "serial/uart.h"
#include "sleep.h"

int main(void){
	// initialize WEFEE	
	initUART();
	init_motor(); 
	init_ultrasensor();
	init_PCINT();
	
	while(1){
		if(BUTTON)	//sleep mode check
			good_night(); // NIGHT NIGHT
		
		ultra_controls();// call the do everything function
	}
}

