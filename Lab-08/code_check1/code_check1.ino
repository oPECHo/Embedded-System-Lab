#include <avr/io.h>
#include <avr/interrupt.h>

void setup()
{
  DDRD |= (1 << DDD6 | 1 << DDD5); // output port D 5-6
  TCCR0B = (0 << WGM02 | 0 << CS02 | 0 << CS01 | 1 << CS00); // CS0 = 001 (หารความถี่ด้วยค่า N = 1)
  TCCR0A = (1 << WGM01 | 1 << WGM00); //WGM0 = 011 = Phase Fast PWM ค่าสูงสุดที่นับได้คือ 0xFF
  TCCR0A |= (1 << COM0A1 | 0 << COM0A0); // COM0A = 10 , TCNT = OCR0A(low) and TCNT = 0x00(High)
  TCCR0A |= (1 << COM0B1 | 0 << COM0B0); // COM0B = 10 , TCNT = OCR0B(low) and TCNT = 0x00(High)

  TCNT0 = 0; // default clock = 0
  OCR0A = 51;
  OCR0B = 179;
}

void loop()
{
  
}
