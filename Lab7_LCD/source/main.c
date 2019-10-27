#include <avr/io.h>
#include "io.h"

int main(void) {
DDRC = 0xFF; PORTC = 0x00; // LCD data lines
DDRD = 0xFF; PORTD = 0x00; // LCD control lines

//Initializes the LCD display
LCD_init();

LCD_DisplayString(1, "Hello World");

while(1) {continue;)
}
