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
#include “simAVRHeader.h”
#endif    

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char PA0 = 0x00; // Temporary variable to hold the value of A0
    unsigned char PA1 = 0x00; // Temporary variable to hold the value of A1
    unsigned char PB0 = 0x00; // Temporary variable to hold the value of B0
    while(1) {
        // 1) Read input
        PA0 = PINA & 0x01;
        PA1 = PINA & 0x02;
        PB0 = !PA0 & PA1;
 /*       if (!PA0 & PA1 == 0x01) { // True if PA0 is 1
            PB0 = (PB0 & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
                             // (clear rightmost 2 bits, then set to 01)
        } else {
            PB0 = (PB0 & 0xFC) | 0x00; // Sets tmpB to bbbbbb00
                             // (clear rightmost 2 bits, then set to 00)
        } 
*/        
    // 3) Write output
    PORTB = PB0;    
    }
    return 0;
}
