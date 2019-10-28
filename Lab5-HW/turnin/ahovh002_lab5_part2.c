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

typedef enum States {start, idle, pressA0, pressA1, reset } States;

int exampleTick(int);

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    States state = start;
    while (1) {
        state = exampleTick(state);
    }
    return 1;
}

int exampleTick(int state) {
	static unsigned char counter;
    unsigned char A0 = (~PINA & 0x01);
    unsigned char A1 = (~PINA & 0x02);
    switch (state) { // Transitions
        case start:
            state = idle;
			counter = 0;
            break;
        case idle:
            if(A0) {
	            state = pressA0;
				if(counter < 9) counter++;
	        } else if(A1) {
	            state = pressA1;
				if(counter > 0) counter--;
            } else if(A0 && A1) {
				state = reset;	
				counter = 0;			
	        } else {
	            state = idle;
            }
            break;
        case pressA0:
           // state = A0 ? pressA0: A0 & A1 ? reset: idle & counter++;
            if(A0 && A1) {
	            state = reset;
				counter = 0;
			} else if(A0) {
               state = pressA0;
            } else if(!A0) {
               state = idle;
            }    
            break;
        case pressA1:
           // state = A1 ? pressA1: A0 & A1 ? reset: idle & counter--;
            if(A0 && A1) {
	            state = reset;
				counter = 0;
            } else if(A1) { 
				state = pressA1; 
            } else if(!A1) {
               state = idle;
            }              
            break;
        case reset:
            state = A0 & A1 ? reset: idle;
            break;
        default:
            state = start;
            break;
    }
    PORTC = counter;
    return state;
}
