#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

volatile uint8_t count;
uint8_t segment_data[] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111   // 9
};

ISR(TIMER1_OVF_vect)
{
    TCNT1 = 3036; // Reset TCNT1 to achieve 1 second overflow
    count++;
    if(count == 10)
    {
        count = 0;
        PORTB ^= (1 << PB5); // Toggle PB5 (LED connected to PB5)
    }

    // Display count on 7-segment LEDs
    PORTC = (segment_data[count] & 0x3F);  // A0-A5 (6 bits)
    if(segment_data[count] & 0x40){  // PD7
      PORTD |= (1 << PD7);
    } else {
      PORTD &= ~(1 << PD7);
    }
}

int main(void)
{
    count = 0;

    // Set PB5 (pin 13 on Arduino UNO) as output for LED
    DDRB |= (1 << DDB5);

    // Set PORTA (A0-A5) and PORTD (PD7) as outputs for 7-segment display
    DDRC = 0x3F;  // A0-A5
    DDRD = 0x80;  // PD7
    DDRD |= (1 << PD7);

    TIMSK1 = (1 << TOIE1); // Enable Timer1 overflow interrupt
    TCCR1A = 0x00;
    TCCR1B = (1 << CS12); // Prescaler = 256, 16 MHz
    TCNT1 = 3036; // Initialize TCNT1 for 1 second overflow

    sei(); // Enable global interrupts

    while(1)
    {}
}
