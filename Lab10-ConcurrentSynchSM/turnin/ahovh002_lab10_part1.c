/*	Author: Arusyak Hovhannesyan ahovh002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://drive.google.com/file/d/1DLzowg_WFKrgcFEtei5yn2NhjDg8gZRV/view?usp=sharing
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_cntcurr = 0;
unsigned long _avr_timer_M = 1;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1= 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
}

typedef enum ThreeLedStates {start, LED0, LED1, LED2} ThreeLedStates;
typedef enum BlinkingLedStates { LED3on, LED3off} BlinkingLedStates;
typedef enum CombineLedStates {wait_100, combine} CombineLedStates;

unsigned char ThreeLEDs = 0x00; //shared output for ThreeLED state machine
unsigned char BlinkingLED = 0x00; // shared output for BlinkingLED state machine

int ThreeLedSM(int tlstate, unsigned short tcount) {
	if (tcount == 0) {// this is for first exercise
	// if (tcount % 30 == 0) { // this is for second exercise
		unsigned char b = 0;
		switch (tlstate) { // Transitions
			case start:
				b = 0x00;
				tlstate = LED0;
			break;
			case LED0:
			    b = 0x01;
				tlstate = LED1;
			break;
			case LED1:
			    b = 0x02;
				tlstate = LED2;
			break;
			case LED2: 
				b = 0x04;
				tlstate = LED0;
			break;
			default:
			   tlstate = start;
			break;
		}
		ThreeLEDs = b;
	}
    return tlstate;
}

int BlinkingLedSM(int blstate, unsigned short tcount) {
	if (tcount == 0) { //this is for first exercise
	// if (tcount % 100 == 0) {
		unsigned char b = 0;
		switch (blstate) { // Transitions
		    case LED3on:
				b = 0x08;
				blstate = LED3off;
				break;
			case LED3off:
				b = 0x00;
				blstate = LED3on;
				break;
			default:
				blstate = LED3on;
				break;
		}
		BlinkingLED = b;
	}
    return blstate;
}

int CombineLedSM(int clstate, unsigned short tcoun) {
    unsigned char b = 0;
    switch (clstate) { // Transitions
		case wait_100:
			if (tcoun == 0) { //this is for first exercise
			//if ((tcoun % 30 == 0)||(tcoun % 100 == 0)) {
				clstate = combine;
			}
        case combine:
            b = BlinkingLED | ThreeLEDs;
            clstate = wait_100;
            break;
        default:
            clstate = wait_100;
            break;
    }
    PORTB = b;
    return clstate;
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;  //output

    ThreeLedStates tlstate = start;
	BlinkingLedStates blstate = LED3on;
	CombineLedStates clstate = wait_100;
	unsigned char cntr = 0;
	TimerSet(1);
    TimerOn();

    while (1) {
		while (!TimerFlag);
		tlstate = ThreeLedSM(tlstate, cntr);
		blstate = BlinkingLedSM(blstate, cntr);
		clstate = CombineLedSM(clstate, cntr);
		TimerFlag = 0;
		//if ((cntr % 2) == 0) PORTB = PORTB + 16;//this is for exercise 3
		if (cntr < 99) cntr++;//this is for first exercise
		if (cntr < 299) cntr++;
		else cntr = 0;
    }
    return 1;
}