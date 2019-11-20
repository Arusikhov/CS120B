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
double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

typedef enum Notes {start, idle, incr, decr } Notes;
typedef enum State {off, on} States;

unsigned char currentNote = 0; //shared variable fed to notes array in musicPlayer SM
unsigned char prevNote = 1;

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


int musicPlayer(int state) {
	unsigned char A0 = (~PINA & 0x02); //button to toggle audio on/off
	switch (state) { // Transitions
		case off:
			if(A0){ 
				PWM_on();
				set_PWM(4*notes[currentNote]);
				state = on;
			}
			PORTB = PORTB & 0xFE;
		break;
		case on:
			PORTB = PORTB | 0x01;
			if(A0) {
				PWM_off();
				state = off;
			} else if(currentNote != prevNote) {
				PWM_on();
				set_PWM(4*notes[currentNote]);
				prevNote = currentNote;
				PORTB = PORTB | 0x02;
			}
		break;
		default:
		state = off;
		break;
	}
	return state;
}

int noteChanger(int note) {
    unsigned char A1 = (~PINA & 0x04);
    unsigned char A2 = (~PINA & 0x08);
	switch (note) { // Transitions
		case start:
		note = idle;
		break;
		case idle:
		if(A1) {
			note = incr;
			PORTB = PORTB | 0x04;
			if(currentNote < 7) currentNote++;
		} else if(A2) {
			note = decr;
			if(currentNote > 0) currentNote--;
		} else {
			note = idle;
		}
		break;
		case incr:
		if(!A1) note = idle;
		break;
		case decr:
		if (!A2) note = idle;
		break;
		default:
		note = start;
		break;
	}
    return note;
};

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input buttons
	DDRB = 0xFF; PORTB = 0x00; //output buzzer
	Notes note = start;
	States state = off;
	while (1) {
		state = musicPlayer(state);
		note = noteChanger(note);
	}
	return 1;
}
