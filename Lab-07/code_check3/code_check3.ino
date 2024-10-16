#include<avr/io.h>
#include<avr/interrupt.h>
uint16_t a;
float val;

void setup() {
  Serial.begin(9600);
  DIDR0 = 0b00111111; // lock digital
  DDRC = 0x00;  // input
  ADMUX = 0x40;  // AVCC
  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Prescale 128 , ADEN = Enable change analog to digital
}

void loop() {
    ADCSRA = 0b11000111; // add start change analog to digital
    while(!( ADCSRA & ( 1 << ADIF) )); // check flag
    a = ADC;
    val = a * ( 5.0 / 1023.0);
    Serial.print("Voltage = ");
    Serial.print(val);
    Serial.println(" V.");
    ADCSRA |= ( 1 << ADIF); // clear flag

    delay(500);
}