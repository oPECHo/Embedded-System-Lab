#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char TB7SEG[] = {
  /*  0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F */

    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000, // 9
    0b10001000, // A
    0b10000011, // B
    0b11000110, // C
    0b10100001, // D
    0b10000110, // E
    0b10001110  // F
};

void initial_read_sw_and_display(void)
{
    unsigned char a;

    DDRB = 0xF0;
    DDRC |= 0x3F;
    DDRB |= (1 << PB4);
    a = PINB;
    a &= 0x0F;
    PORTC = ~ (TB7SEG[a]);
    if (TB7SEG[a] & 0x40) {
        // ถ้าบิตที่ 6 ของ TB7SEG[a] เป็น 1 ให้ตั้งค่า PB4 ของ PORTB เป็น 0
        PORTB &= ~(1 << PB4);
    } else {
        // ถ้าบิตที่ 6 ของ TB7SEG[a] เป็น 0 ให้ตั้งค่า PB4 ของ PORTB เป็น 1
        PORTB |= (1 << PB4);
    }

}

int main(void)
{

    sei(); // Enable global interrupts
    PCICR = 0x01; // Enable pin change interrupt for PCINT0-PCINT7
    PCMSK0 = 0x0F; // Enable PCINT0-PCINT3 for pin change interrupt
    initial_read_sw_and_display(); // Initial read and display

    while(1)
    {
    }
}

ISR(PCINT0_vect)
{
    unsigned char a;

    DDRB = 0xF0;
    DDRC |= 0x3F;
    DDRB |= (1 << PB4);
    a = PINB;
    a &= 0x0F;
    PORTC = ~ (TB7SEG[a]);
    if (TB7SEG[a] & 0x40) {
        // ถ้าบิตที่ 6 ของ TB7SEG[a] เป็น 1 ให้ตั้งค่า PB4 ของ PORTB เป็น 0
        PORTB &= ~(1 << PB4);
    } else {
        // ถ้าบิตที่ 6 ของ TB7SEG[a] เป็น 0 ให้ตั้งค่า PB4 ของ PORTB เป็น 1
        PORTB |= (1 << PB4);
    }
}
