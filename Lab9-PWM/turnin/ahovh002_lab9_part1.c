/*	Author: ahovh002
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency; //keeps track of the currently set frequency
	//will only update the registers when the frequency changes, otherwise allows
	//music to play uninterrupted 
	if(frequency != current_frequency) {
		if(!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else {TCCR3B |= 0x03; } //resumes/continues timer/counter
		
		if(frequency < 0.954) { OCR3A = 0xFFFF; }
			
		else if(frequency > 31250) { OCR3A = 0x0000; }
			
		else { OCR3A = (short)(8000000/ (128*frequency)) -1; }
		
		TCNT3 = 0; //resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1<< COM3A0);
	TCCR3B = (1<< WGM32) | (1<<CS31) | (1<<CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; // Configure port B's 8 pins as outputs, initialize to 0s
    /* Replace with your application code */
	unsigned char buttns;
	unsigned char old_buttns = 0x00;
    while (1) 
    {
		buttns = (~PINA) & 0x0E;
		if (old_buttns != buttns) {
			if (buttns == 0x02){
				PWM_on();
				set_PWM(261.63);
			} else if (buttns == 0x04){
				PWM_on();
				set_PWM(293.66);
			} else if (buttns == 0x08){
				PWM_on();
				set_PWM(329.63);
			} else
				PWM_off();
			old_buttns = buttns;
		  }
    }
}

