/*	Author: Arusyak Hovhannesyan
 *	Lab Section: 
 *	Assignment: Lab 3#  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif    

int main(void) {
   DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
   DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
   unsigned char tmpC = 0x00; // Temporary variable to hold the value

   while(1) {
      if (PINA & 0x0F) <3) tmpC = 0x60;
      else if ((PINA & 0x0F) <5) tmpC = 0x70;
      else if ((PINA & 0x0F) <7) tmpC = 0x38;
      else if ((PINA & 0x0F) <10) tmpC = 0x3c;
      else if ((PINA & 0x0F) <13) tmpC = 0x3e;
      else tmpC = 0x3F;
      
    // Write output
    PORTC = tmpC;    
    }
    return 0;
}
