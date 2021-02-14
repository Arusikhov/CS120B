/*	Author: Arusyak Hovhannesyan
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #4
 *	Exercise Description: 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://drive.google.com/file/d/1DAmAGxIo8HQWgV_0rt5jhwjK8ZQEd3hM/view?usp=sharing
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
	OCR1A = 12;
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
typedef enum BlinkingLedStates { LED3on, LED3on} BlinkingLedStates;
typedef enum CombineLedStates {wait_100, combine} CombineLedStates;
typedef enum FrequencyAdjusterStates {fstart, idle, pressA0, pressA1} FrequencyAdjusterStates;
typedef enum synchSMStates { dwn_state, up_state} synchSMStates;
unsigned char ThreeLEDs = 0x00; //shared output for ThreeLED state machine
unsigned char BlinkingLED = 0x00; // shared output for BlinkingLED state machine
unsigned short curr_frequency = 0; //global variable to store the current frequency called in FrequencyAdjusterStates state machine
unsigned char Buzzer = 0x00; //shared output for ThreeLED state machine

int ThreeLedSM(int tlstate) {//, unsigned short tcount) {
	//if (tcount % 300 == 0) {
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
	//}
    return tlstate;
}

int BlinkingLedSM(int blstate) {//, unsigned short tcount) {
	//if (tcount % 1000 == 0) {
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
	//}
    return blstate;
}

int FrequencyAdjusterSM(int fastate) { //, unsigned short tcount) {
	unsigned char A0 = (~PINA & 0x01);
   unsigned char A1 = (~PINA & 0x02);
   //if (tcount % 50 == 0) {
      //unsigned char b = 0;
		switch (fastate) { // SM
         case fstart:
            fastate = idle;
            break;
         case idle:
            if(A0) {
	            fastate = pressA0;			
	         } else if(A1) {
				fastate = pressA1;
			 } else{
	            fastate = idle;
            }
            break;
         case pressA0:
            if(A0) {
               fastate = pressA0;
            } else if(!A0) {
				if (curr_frequency < 25) {
					curr_frequency++;
				}
               fastate = idle;
            }    
            break;
         case pressA1:
            if(A1) { 
               fastate = pressA1; 
            } else if(!A1) {
				if (curr_frequency > 0){
					curr_frequency--;
				}
               fastate = idle;
            }              
            break;
         default:
            fastate = start;
            break;
      }
      //curr_frequency = b;
   //}
    return fastate;
}

int Buzzer_driver(int synchSM) {//, unsigned short tcount) {
   unsigned char A2 = (~PINA & 0x04);
	static unsigned short tcount;
	if (tcount >= curr_frequency) {
		tcount = 0;
		if (A2) {
			switch (synchSM) { // Transitions
				case dwn_state:
					Buzzer = 0x10;
					synchSM = up_state;
				break;
				case up_state:
					Buzzer = 0x00;
					synchSM = dwn_state;
				break;
				default:
				synchSM = dwn_state;
				Buzzer = 0x00;
				break;
			}
		}
	} else {
		tcount ++;
	}
	return synchSM;
}


int CombineLedSM(int clstate) { //, unsigned short tcoun) {
    unsigned char b = 0;
    //switch (clstate) { // Transitions
	//	case wait_100:
			//if ((tcoun % 50)==0) {
				clstate = combine;
			//}
        //case combine:
            b = ThreeLEDs | BlinkingLED | Buzzer;
            //clstate = wait_100;
           // break;
        //default:
            //clstate = wait_100;
            //break;
   // }
    PORTB = b;
    return clstate;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;  //output
	DDRA = 0x00; PORTA = 0xFF; //input
	ThreeLedStates tlstate = start;
	BlinkingLedStates blstate = LED3on;
	CombineLedStates clstate = wait_100;
	FrequencyAdjusterStates fastate = fstart;
	synchSMStates synchSM = dwn_state;
	unsigned short cntr = 2;
	TimerSet(1);
	TimerOn();

    while (1) {
		while (!TimerFlag);
		if (cntr % 300 == 0) {
			tlstate = ThreeLedSM(tlstate); //, cntr);
			//clstate = CombineLedSM(clstate); //, cntr);
		}
		if (cntr % 1000 == 0) {
			blstate = BlinkingLedSM(blstate); //, cntr);
			//clstate = CombineLedSM(clstate); //, cntr);
		}
		if (cntr % 50 == 0) {
			fastate = FrequencyAdjusterSM(fastate); //, cntr);
		}
		synchSM = Buzzer_driver(synchSM);
		clstate = CombineLedSM(clstate); //, cntr);
		TimerFlag = 0;
		//if ((~PINA & 0x04)&&(cntr % 2) == 0) PORTB = PORTB + 16;//this is for exercise 3
		if (cntr < 2999) cntr++;
		else cntr = 0;
    }
    return 1;
}