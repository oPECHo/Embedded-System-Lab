#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char TB7SEG[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
};

volatile unsigned char count = 0;

void display_count(unsigned char num) {
    PORTC = (TB7SEG[num]);
    if (TB7SEG[num] & 0x40) {
      PORTB |= (1 << PB4);  
    } else {
      PORTB &= ~(1 << PB4); 
    }

}

void setup() {
    DDRB = 0xF0; // PB0 as output for segment 'A-G', PB1 as output for transistor
    DDRC = 0x3F; // PC0-PC5 as output for 7-segment display
    DDRB |= (1 << PB4);
    // Set up interrupt
    EIMSK |= 0x01; // Enable INT0
    EICRA |= 0x02; // Trigger on falling edge of INT0
    sei();
}

int main(void) {
    setup();
    display_count(count); // Display the initial count

    while (1) {
    }
}

ISR(INT0_vect) {
    count = (count + 1) % 10; // Increment count and wrap around to 0 after 9
    display_count(count); // Display the updated count
}
