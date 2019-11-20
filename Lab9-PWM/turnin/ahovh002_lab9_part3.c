#include<avr/io.h>
#include<avr/interrupt.h>
#include "simAVRHeader.h"
#endif

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

double note[8] = {261.63, 293.66, 329.63, 349.23, 369.99, 392.00, 440.00, 493.88, 523.25}
double note_duration
double pause
double pause_duration

int main(void)
{
	DDRB = 0xFF; // Configure port B's 8 pins as outputs
	unsigned char buttns;
	unsigned char old_buttns = 0x00;
	while (1)
	{
		buttns = (~PINA) & 0x0E;
		if (old_buttns != buttns) {
			if (buttns == 0x02){
				PWM_on();
				set_PWM(261.63);
				} else if (buttns == 0x04){
				PWM_on();
				set_PWM(293.66);
				} else if (buttns == 0x08){
				PWM_on();
				set_PWM(329.63);
			} else
			PWM_off();
			old_buttns = buttns;
		}
	}
}
