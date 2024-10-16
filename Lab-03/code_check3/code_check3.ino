#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

signed char PressedKey;
signed char n;

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
    0b01110111, // A (10)
    0b01111100, // b (11)
    0b00111001, // C (12)
    0b01011110, // d (13)
    0b01111001, // E (14)
    0b01110001  // F (15)
};

void display_7_segment(signed char num)
{
    if(num >= 0 && num <= 15)
    {
        PORTC = TB7SEG[num];
        if (TB7SEG[num] & 0x40) {
            PORTB |= (1 << PB4);  
        } else {
            PORTB &= ~(1 << PB4); 
        }
    }
}

int main(void)
{
    DDRD = 0b11110000;    // Set PD4-PD7 as output for keypad rows
    DDRB = 0b00010000;    // Set PB4 as output for 7-segment display segment G
    DDRC = 0b01111111;    // Set PC0-PC6 as output for 7-segment display
    EICRA = 0b00001000;   // Set INT1 to trigger on falling edge
    EIMSK = 0x02;         // Enable INT1
    sei();                // Enable global interrupts
    PORTB = 0x0F;         // Enable pull-up resistors on PB0-PB3
    PORTD = 0b00001000;   // Initialize PORTD, PD3 high
    PORTC = 0b00000000;   // Initialize PORTC to 0 (7-segment off)
    
    while(1)
    {} 
}

ISR(INT1_vect)
{
    PressedKey = -1;
    n = 1;
    do
    {
        if(n == 1)
        {
            PORTD = 0b11100000; // Set PD4 low
        }
        else if(n == 2)
        {
            PORTD = 0b11010000; // Set PD5 low
        }
        else if(n == 3)
        {
            PORTD = 0b10110000; // Set PD6 low
        }
        else if(n == 4)
        {
            PORTD = 0b01110000; // Set PD7 low
        }
        _delay_ms(10);
        
        switch(PINB & 0x0F)
        {
            case 0b0111:
                if(n == 1) PressedKey = 10;   // A
                if(n == 2) PressedKey = 3;    // 3
                if(n == 3) PressedKey = 2;    // 2
                if(n == 4) PressedKey = 1;    // 1
                break;
            case 0b1011:
                if(n == 1) PressedKey = 11;   // B
                if(n == 2) PressedKey = 6;    // 6
                if(n == 3) PressedKey = 5;    // 5
                if(n == 4) PressedKey = 4;    // 4
                break;
            case 0b1101:
                if(n == 1) PressedKey = 12;   // C
                if(n == 2) PressedKey = 9;    // 9
                if(n == 3) PressedKey = 8;    // 8
                if(n == 4) PressedKey = 7;    // 7
                break;
            case 0b1110:
                if(n == 1) PressedKey = 13;   // D
                if(n == 2) PressedKey = 15;   // F
                if(n == 3) PressedKey = 0;    // 0
                if(n == 4) PressedKey = 14;   // E
                break;
        }
        n++;
    } while (n <= 4);

    if(PressedKey != -1)
    {
        display_7_segment(PressedKey);
    }
    
    PORTD = 0b00001000; // Reset PORTD to original state, PD3 high
}
