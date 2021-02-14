/*	Author: Arusyak Hovhannesyan
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: my own melody(Arax) on buzzer
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://drive.google.com/file/d/1CoPov9_TpGZOPvMJIh20ahsYnmJrRWUw/view?usp=sharing
 */
#include<avr/io.h>
#include<avr/interrupt.h>

#define c (523.25 * 4)
#define b (493.88 * 4)
#define a (440.00 * 4)
#define g (412.00 * 4)
#define f (350.23 * 4)
#define e (329.63 * 4)
#define d (293.66 * 4)


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

double melody[16] = {0, c, b, c, a, b, a, g, f, e, f, g, a, b, g, a};
double note_duration[16] = {0, 5, 2, 2, 2, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 5};
double pause_duration[16] = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0};

typedef enum State {wait, setNote, noteDuration, pauseDuration} States;

int melodyPlayer(int);
	
int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input buttons
	DDRB = 0xFF; PORTB = 0x00; //output buzzer
	States state = wait;
	TimerSet(10);
	TimerOn();
	while (1) {
		while (!TimerFlag);
		state = melodyPlayer(state);
		TimerFlag = 0;
	}
	return 1;
}

int melodyPlayer(int state) {
	static unsigned char durationCount;
	static unsigned char i = 0;
	unsigned char A0 = (~PINA & 0x01);
	switch (state) {
		case wait:
			if(!A0) {state = wait;}
			else if (A0) {state = setNote;}
			i = 0; durationCount = 0;
			break;
		case setNote:
			if(i == 16){
				state = wait;
			} else {
				i++;
				PWM_on();
				set_PWM(melody[i]);
				durationCount = 0;
				state = noteDuration;
			}
			break;
		case noteDuration:
			if(i == 16){
				PWM_off();
				state = wait;
			} else if(durationCount <= note_duration[i]) {
				durationCount++;
				state = noteDuration;
			} else if(pause_duration[i] == 0) {
				durationCount = 0;
				state = setNote;
			} else {
				durationCount = 0;
				PWM_off();
				state = pauseDuration;
			}
		break;
		case pauseDuration:
		if(i == 16){
			PWM_off();
			state = wait;
		} else if(durationCount <= pause_duration[i]){
			durationCount++;
			state = pauseDuration;
		} else if(i <= 15){
			durationCount = 0;
			PWM_on();
			state = setNote;
		}
		break;
		default:
		state = wait;
		break;
	}
	return state;
};
