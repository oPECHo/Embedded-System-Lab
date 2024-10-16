#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

ISR(TIMER0_OVF_vect)
{
  TCNT0 = 178;
  PORTB ^= 0x01; // Toggle PB0 (Oscilloscope connected to PB0)
}

int main(void)
{
  // Set PB0 (pin 8 on Arduino UNO) as output for Oscilloscope
  DDRB |=  (1 << DDB0);

  TIMSK0 |= (1<<TOIE0); // Enable Timer0 overflow interrupt
  TCCR0A = 0x00;
  TCCR0B |= (1<<CS02 | 1<<CS00); //Prescaler = 1024, 16 MHz
  TCNT0 = 178;

  sei();

  while(1)
  {} 
}
