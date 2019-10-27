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

typedef enum States {start, releaseLED0, pressLED0, releaseLED1, pressLED1} States;

int exampleTick(int);

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    States state = start;
    while (1) {
        state = LED_switch(state);
    }
    return 1;
}

int LED_switch(int state) {
    unsigned char A0 = (~PINA & 0x01);
	unsigned char b = 0;
    switch (state) { // Transitions
        case start:
            state = releaseLED0;
			b = 0x01;
            break;
        case releaseLED0:
			b = 0x01;
            if(A0) {
	            state = pressLED1;
			}
            break;
        case pressLED1:
			b = 0x02;
            if(!A0) state = releaseLED1;
            break;
        case releaseLED1:
			b = 0x02;
           // state = A1 ? pressA1: A0 & A1 ? reset: idle & counter--;
            if(A0) {
				state = pressLED0;
			}           
            break;
        case pressLED0:
			b = 0x01;
            if(!A0) state = releaseLED0;
            break;
        default:
            state = start;
            break;
    }
    PORTC = b;
    return state;
}
