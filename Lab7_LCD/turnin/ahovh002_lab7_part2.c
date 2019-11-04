#include<avr/io.h>
#include<avr/interrupt.h>
#include "io.h"
#define buttonPressed 0x01
#define timerInterrupt 0x00

//#define SREGSS _SFR_MEM8(0x3F)

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

typedef enum States {start, PBa0, PBa1, PBa2, gameOver} States;
unsigned char dir = 0;
unsigned char score = 0; //LED Game Score

int main() {
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	unsigned char buttonReg = 0;
	unsigned char button = 0;
	States state = start;
	
	LCD_init();
	LCD_ClearScreen();
	TimerSet(30);
	TimerOn();
	while(1) {
		while (!TimerFlag) {
			buttonReg = button;
			button = ~PINA & 0x01;
			if(button & !buttonReg) {
				TimerOff();
				state = LED_Game(state, buttonPressed);
			}
		}
		state = LED_Game(state, timerInterrupt);
		TimerFlag = 0;
	}
}

int LED_Game(int state, int source) {
	switch (state) { // Transitions
		case start:
		state = PBa0;
		dir = 0;
		score = 5;
		LCD_Cursor(1);
		LCD_WriteData(score+0x30);
		break;
		
		case PBa0:
		if(source == buttonPressed) {
			if (score > 0) {
				score--;
				LCD_Cursor(1);
				LCD_WriteData(score+0x30);
			}
			state = gameOver;
		} else {
			state = PBa1;
			dir = 0;
		}
		break;
		case PBa1:
		if(source == buttonPressed) {
			state = gameOver;
			if(score < 9) {
				score++;
				LCD_Cursor(1);
				LCD_WriteData(score+0x30);				
				if (score == 9) {
					LCD_DisplayString(1," Victory");
				}
			}
		} else if(!dir) {
				state = PBa2;
		} else {
			state = PBa0;
		}
		break;
		case PBa2:
		if(source == buttonPressed) {
			if (score > 0) {
				score--;
				LCD_Cursor(1);
				LCD_WriteData(score+0x30);
			}
			state = gameOver;
		} else {
			state = PBa1;
			dir = 1;
		}
		break;
		case gameOver:
		if(source == buttonPressed) {
			if (score == 9)	{
				score = 5;
				LCD_ClearScreen();
			}
			LCD_Cursor(1);
			LCD_WriteData(score+0x30);
			TimerSet(30);
			TimerOn();
			state = PBa0;
		}
		break;
		default:
		state = start;
		break;
	}
	switch (state) { // Actions
		case start:
		break;
		case PBa0:
		PORTC = 0x01;
		break;
		case PBa1:
		PORTC = 0x02;
		break;
		case PBa2:
		PORTC = 0x04;
		break;
		case gameOver:
		break;
		default:
		break;
	}
	return state;
}
