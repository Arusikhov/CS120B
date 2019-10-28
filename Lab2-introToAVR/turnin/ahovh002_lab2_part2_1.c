#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
//    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    unsigned char PA = 0x00; // Temporary variable to hold the value of A0

    unsigned char cntavail = 0x00; // Temporary variable to hold count of lots available in parking lot
    while(1) {
        // 1) Read input
        PA = PINA & 0x0F;
        PORTC = cntavail;
        cntavail = !(PA & 0x01) + !((PA & 0x02)>>1) + !((PA & 0x04)>>2) + !((PA & 0x08)>>3);


        // 3) Write output
        if (cntavail == 0x00) {
            PORTC = 0x80; //sets 7th bit to 1 if parking lot is full
        } else {
            PORTC = cntavail; //sets count of parking available to port c
        }
    }
    return 0;
}
