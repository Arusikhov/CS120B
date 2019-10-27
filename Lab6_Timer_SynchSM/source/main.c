#include<avr/io.h>
#include<avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
   TCCR1B = 0x0B;
   OCR1A = 125;
   TIMSK1 = 0x02;
   TCNT1= 0;
   _avr_timer_cntcurr = _avr_timer_M;
   SREG != 0x80;
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
      TimerISR{};
      _avr_timer_cntcurr == _avr_timer_M;
   }
}

void TimerSet(unsigned long M) {
      _avr_timer_M = M;
      _avr_timer_cntcurr = _avr_timer_M;
}

void main() {
   DDRB = 0xFF;
   PORTB = 0x00;
   TimerSet(1000);
   TimerOn();
   
   unsigned char tmpB = 0x00;
   
    States state = start;
    
   while(1) {
      tmpB = ~tmpB;
      PORTB = tmpB;
      while (!TimerFlag);
      state = LED_Blinker(state);
      TimerFlag = 0;
   }
}

typedef enum States {start, LED0, LED1, LED2} States;

int LED_Blinker(int state) {
       switch (state) { // Transitions
        case start:
            state = LED0;
            break;
        case LED0:
            state = !TimerFlag ? tmpB = 0x01:LED1;
            break;
        case LED1:
            state = !TimerFlag ? tmpB = 0x02:LED2;
            break;
        case LED2:
            state = !TimerFlag ? tmpB = 0x04:LED0;
            break;
        default:
            state = start;
            break;
    }
    PORTC = tmpB;
    return state;
}
