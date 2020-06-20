#ifndef SLEEP_H
#define SLEEP_H

/**
* @defgroup soundIndic Sound Wall Collision Warning
* @brief Defines the use of the sound wall collision warning system
* @{
*/
#define SOUND_PORT		PORTC5						//!< sound port (PORTC5)
#define TONER_UPPER			PORTC |= (1<<SOUND_PORT)	//!< tone up sound (PORTC5)
#define TONE_DOWN_FOR_WHAT		PORTC &= ~(1<<SOUND_PORT)	//!< tone down sound (PORTC5)
/**@}*/



void init_PCINT(){
	PCICR |= (1 << PCIE0);			// set PCIE0 to enable PCMSK0 scan
	PCMSK0 |= (1 << PCINT4);		// set PCINT4 to trigger an interrupt on state change
	DDRC |= (1<<SOUND_PORT);		// sound indicator
	sei();
}

void good_night(){
	int i;
	for(i = 0; i < 600; i++){
		TONER_UPPER; //start tone
		_delay_ms(1);
		TONE_DOWN_FOR_WHAT; //mute tone
		_delay_ms(1);
	}
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // go to sleep
	cli();								
	
	sleep_enable();
	sei();
	sleep_cpu();
	
	cli();
	PCICR |= (1 << PCIE0);     // set PCIE0 to enable PCMSK0 scan
	PCMSK0 |= (1 << PCINT4);   // set PCINT0 to trigger an interrupt on state change
	sleep_disable();
	sei();
}
#endif