#include <avr/io.h>
#include <avr/interrupt.h>

void setup()
{
  DDRD |= (1 << DDD6 | 1 << DDD5); // output port D 5-6
  TCCR0B = (0 << WGM02 | 0 << CS02 | 1 << CS01 | 1 << CS00); // CS0 = 001 (หารความถี่ด้วยค่า N = 1)
  TCCR0A = (0 << WGM01 | 1 << WGM00); //WGM0 = 01
  TCCR0A |= (1 << COM0A1 | 0 << COM0A0); // COM0A = 10
  TCCR0A |= (1 << COM0B1 | 0 << COM0B0); // COM0B = 10

  TCNT0 = 0; // default clock = 0
  OCR0A = 204;
  OCR0B = 77;
}

void loop()
{
  
}
