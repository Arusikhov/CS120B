#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

volatile unsigned char tcntr; // 20ms counter
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_cntcurr = 0;
unsigned long _avr_timer_M = 1;

int LCD_Counter(int state);

void TimerOn() {
	TCCR1B = 0x08;
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

typedef enum States {start, idle, incr, decr, reset} States;
unsigned char Data = 0;

int main() {
	DDRA = 0x00; PORTA = 0xFF; // buttons
	DDRD = 0xFF; PORTD = 0x00; // LCD data lines
	DDRB = 0xFF; PORTB = 0x00; // LCD control lines  
	DDRC = 0xFF; PORTC = 0x00;
	States state = start;
	
	TimerSet(2);
	TimerOn();
	
	while(1) {
		PORTC = tcntr;
		while (!TimerFlag) {
		}
		state = LCD_Counter(state);
		
		TimerFlag = 0;
		//TimerSet(1);
	}
}


int LCD_Counter(int state) { //state machine
	LCD_init();
	unsigned char incrButton = ~PINA & 0x01;
	unsigned char decrButton = ~PINA & 0x02;
	switch (state) { // Transitions
		case start:
			state = idle;
			Data = 0;
		break;
		
		case idle:
			if(incrButton && decrButton) {
				state = reset;
			} else if(incrButton) {
				if (Data < 9) Data++;
				state = incr;
			} else if(decrButton) {
				if (Data > 0) Data--;
				state = decr;
			}
		break;
		case incr:
			if(incrButton && decrButton) {
				state = reset;
				} else if(incrButton) {
				state = incr;
				} else {
				state = idle;
			}
		break;
		case decr:
			if(incrButton && decrButton) {
				state = reset;
				} else if(decrButton) {
				state = decr;
				} else  {
				state = idle;
			}
		break;
		case reset:
			if(!incrButton && !decrButton) {
				state = idle;
				} else {
				state = reset;
			}
		break;
		default:
			state = start;
		break;
	}
	switch (state) { // Actions
		case start: 
		tcntr = 0;
		break;
		case idle: 	
		tcntr = 0;
		break;
		case incr:
		tcntr++;
		if((Data < 9) && (tcntr ==50)) {
			Data++;
			tcntr = 0;
		}
		break;
		case decr:
		tcntr++;
		if((Data > 0) && (tcntr ==50)) {
			Data--;
			tcntr = 0;
		}
		break;
		case reset:
		tcntr = 0;
		Data = 0;
		break;
		default:
		break;
	} //end actions
	LCD_Cursor(1);
	LCD_WriteData(Data+0x30);
	return state;
} //end state machine


