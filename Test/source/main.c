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
#include <simAVRHeader.h>
#endif    

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    //   DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char PAA = 0x00; // Temporary variable to hold the value of A0
    unsigned char PAB = 0x00;
    unsigned char PAC = 0x00;
    unsigned char PAD = 0x00;
    unsigned char cntavail = 0x00; //Temporary variable to hold the value of cntavail
    unsigned char i = 0x00;
    while(1) {
      i = 0x04;
        // 1) Read input
        PAA = PINA & 0x01;
	PAB = PINA & 0x02;
        PAC = PINA & 0x04;
	PAD = PINA & 0x08;
        if(PAA) i-0x01;
	if(PAB) i-0x01;
	if(PAC) i-0x01;
	if(PAD) i-0x01;
	cntavail = i;
           
     // 3) Write output
    PORTC = cntavail;    
    }
    return 0;
}
