/*	Author: ahovh002
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdio.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef enum States {start, locked, press3, release3, unlockDoor } States;

int exampleTick(int);

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

    States state = start;
    while (1) {
        state = exampleTick(state);
    }
    return 1;
}

int exampleTick(int state) {
	
	  enum state_in
	  {
		  zero, 
		  one,
		  two,
		  three,
		  four
	  };

	  enum state_in b;
	
	unsigned char c = 0x00;
	//unsigned char b = 0x00;
    unsigned char buttonx = (~PINA & 0x01);
    unsigned char buttony = (~PINA & 0x04);
	unsigned char button3 = (~PINA & 0x02);
	unsigned char indoorButton = (~PINA & 0x08);
    switch (state) { // Transitions
        case start:
			c = 0x00;
            state = locked;
			b = zero;
            break;
        case locked:
		    b = one;
            if(button3) {
	            state = press3;
			} 
            break;
        case press3:
		    b = two;
            if(!button3 && !buttonx && !buttony) {
	            state = release3;
			} else if (button3 && !buttonx && !buttony){
				state = press3;
			} else {
	            state = locked;
            } 
            break;
        case release3:
			b = three;
            if(!button3 && !buttonx && buttony) {
	            state = unlockDoor;
	            } else if (!button3 && !buttonx && !buttony){
	            state = release3;
	            } else {
	            state = locked;
            }       
            break;
        case unlockDoor:
			b = four;
			c = 0x01;
			if(indoorButton) state = locked;
            break;
        default:
            state = start;
            break;
    }
    PORTC = c;
	PORTB = b;
    return state;
}
