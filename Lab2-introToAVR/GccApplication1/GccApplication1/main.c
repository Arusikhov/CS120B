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
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char PA = 0x00; // Temporary variable to hold the value of A0
//    unsigned char PA1 = 0x00; // Temporary variable to hold the value of A1
    unsigned char PB = 0x00; // Temporary variable to hold the value of B0
    unsigned char cntavail = 0x00;
    while(1) {
        // 1) Read input
        PA = PINA & 0x0F;
        //PB0 = !PA0 & PA1;
        PORTC = cntavail;
        cntavail = !(PA & 0x01) + !((PA & 0x02)>>1) + !((PA & 0x04)>>2) + !((PA & 0x08)>>3);
           
        
    // 3) Write output
    PORTB = PB;    
    }
    return 0;
}
