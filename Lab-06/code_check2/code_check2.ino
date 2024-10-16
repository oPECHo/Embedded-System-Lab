#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/delay.h>
#define TURN_ALL_LED_OFF 0x00
#define ADC5 5
unsigned char TABLE7SEG[] =
{
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
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001, // F
  0b00000000
};

int counter = 0;
int adcVal, prevADC, DECODED;

void DISPLAY7segment(signed char a)
{
  PORTD = TABLE7SEG[a];
}

unsigned int ADC_read(unsigned char a)
{
  ADMUX &= 0xF0;
  ADMUX |= a;
  ADCSRA |= (1<<ADSC);
  while( !(ADCSRA & (1<<ADIF)) );
  ADCSRA |= (1<<ADIF);
  return ADC;
}

int main(void)
{
  DDRC = 0x00;
  DDRD = 0xFF;
  DISPLAY7segment(TURN_ALL_LED_OFF);
  ADMUX = 0b01000101;
  ADCSRA = 0b10000111;
  DECODED = 0;
  while(1)
  {
    counter = 0;
    prevADC = 0;
    do
    {
      adcVal = ADC_read(ADC5);
      if((abs(adcVal-prevADC) < 6) && (adcVal > 30))
        counter++;
      else
        counter = 0;
      _delay_ms(3);
      prevADC = adcVal;
    }
    while (counter < 20);
    if((adcVal >= 30) && (adcVal <= 51))          DECODED = 1;
    else if((adcVal >= 71) && (adcVal <= 82))     DECODED = 2;
    else if((adcVal >= 114) && (adcVal <= 122))   DECODED = 3;
    else if((adcVal >= 143) && (adcVal <= 153))   DECODED = 10; //A

    else if((adcVal >= 104) && (adcVal <= 110))   DECODED = 4;
    else if((adcVal >= 184) && (adcVal <= 205))   DECODED = 5;
    else if((adcVal >= 245) && (adcVal <= 276))   DECODED = 6;
    else if((adcVal >= 327) && (adcVal <= 337))   DECODED = 11; //B

    else if((adcVal >= 168) && (adcVal <= 173))   DECODED = 7;
    else if((adcVal >= 286) && (adcVal <= 296))   DECODED = 8;
    else if((adcVal >= 368) && (adcVal <= 388))   DECODED = 9;
    else if((adcVal >= 450) && (adcVal <= 491))   DECODED = 12; //C

    else if((adcVal >= 757) && (adcVal <= 797))   DECODED = 14; // * --> E
    else if((adcVal >= 879) && (adcVal <= 900))   DECODED = 0;
    else if((adcVal >= 920) && (adcVal <= 937))   DECODED = 15; // # --> F
    else if((adcVal >= 941) && (adcVal <= 1023))  DECODED = 13; //D

    do
    {
      adcVal = ADC_read(ADC5);
    }
    while(adcVal > 30);
    DISPLAY7segment(DECODED);
  }
}

