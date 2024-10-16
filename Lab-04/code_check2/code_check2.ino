#include <avr/io.h>
#define F_CPU 16000000UL

int main(void)
{
  // Set PD6 (pin 6 on Arduino UNO) as output for Oscilloscope
  DDRD |=  (1 << DDD6);

  TIMSK0 |= (1 << OCIE0A); //OCRnA Enable
  TCCR0A = (1 << COM0A0 | 1 << WGM01); //การทำงานแบบ CTC
  TCCR0B = (1 << CS02 | 1 << CS00); //Prescaler = 1024, 16 MHz
  OCR0A = 77;

  while(1)
  {} 
}