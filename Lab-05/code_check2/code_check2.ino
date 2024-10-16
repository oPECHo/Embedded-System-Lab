#include <avr/io.h>
#include <math.h>

#define F_CPU 16000000UL

void setup() {
  // Disable digital input buffers on ADC0-ADC5
  DIDR0 = 0b00111111; 
  // Set PORTD as output
  DDRD = 0xFF;
  // Set PORTC as input (for ADC)
  DDRC = 0x00;

  // Set ADC reference to AVcc and select ADC0 (A0) as input
  ADMUX = 0b01000000;

  // Enable ADC, set prescaler to 128
  ADCSRA = 0b10000111;
}

void loop() {
  // Start ADC conversion
  ADCSRA |= (1 << ADSC);
  // Wait for conversion to complete
  while (!(ADCSRA & (1 << ADIF))) {}
  // Map the ADC value (0-1023) to a step value (0-8)
  int step = (((ADC / 1023) * 255 ) / 255) * 8;
  // Set PORTD to light up the corresponding LEDs
  PORTD = round(pow(2, step) - 1);
  // Clear ADC interrupt flag
  // ADCSRA ^= (1 << ADIF);
}
