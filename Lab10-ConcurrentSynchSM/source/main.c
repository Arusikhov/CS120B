/*	Author: ahovh002
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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
typedef enum BlinkingLedStates { LED3, BIT0} BlinkingLedStates;
typedef enum CombineLedStates {combine } CombineLedStates;

unsigned char ThreeLEDs = 0x00; //shared output for ThreeLED state machine
unsigned char BlinkingLED = 0x00; // shared output for BlinkingLED state machine

int ThreeLedSM(int state) {
    unsigned char b = 0;
    switch (state) { // Transitions
        case start:
            b = 0x00;
            state = LED0;
            break;
        case LED0:
            b = 0x01;
state = LED1;
            break;
        case LED1:
            b = 0x02;
state = LED2;
            break;
        case LED2: 
            b = 0x04;
            state = LED0;
            break;
        default:
            state = start;
            break;
    }
    ThreeLEDs = b;
    return state;
}

int BlinkingLedSM(int state) {
    unsigned char b = 0;
    switch (state) { // Transitions
        case LED3:
            b = 0x08;
            state = LED0;
            break;
        case BIT0:
            b = 0x01;
            state = LED3;
            break;
        default:
            state = LED3;
            break;
    }
    BlinkingLED = b;
    return state;
}

int CombineLedSM(int state) {
    unsigned char b = 0;
    switch (state) { // Transitions
        case combine:
            b = BlinkingLED | ThreeLEDs;
            state = combine;
            break;
        default:
            state = combine;
            break;
    }
    PORTB = b;
    return state;
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;  //output

    ThreeLedSM state = start;

    TimerSet(100);
    TimerOn();

    while (1) {
	while (!TimerFlag);
	state = ThreeLedSM(state);
        state = BlinkingLedSM(state);
        state = CombineLedSM(state);
	TimerFlag = 0;
    }
    return 1;
}



