#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect){
  TCNT0 = 32;
  PORTC ^= 0x01;
}

int main() {
  DDRC |= (1 << DDC0);
  TIMSK0 |= (1 << TOIE0);
  TCCR0A = 0x00;
  TCCR0B |= (1 << CS01);
  TCNT0 = 32;
  sei();

  while(1){}   
}
