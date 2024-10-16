#define TURN_OF_LED 0x00
#define ADC5 5
#define F_CPU 16000000UL

unsigned int ADC_read(unsigned char a){
  ADMUX &= 0xF0;
  ADMUX |= a;
  ADCSRA |= (1<<ADSC);
  while (!(ADCSRA & (1<<ADIF)));
  ADCSRA |= 0<<ADIF;
  return ADC;
}

unsigned char TABLE7SEG[]= {
  0b00111111, 0b00000110,
	0b01011011, 0b01001111,
	0b01100110,	0b01101101,
	0b01111101, 0b00000111,
	0b01111111, 0b01101111,
	0b01110111, 0b01111100,
	0b00111001, 0b01011110,
	0b01111001, 0b01110001
};

int counter = 0;
int adcVal,prevADC,DECODED;

void DISPLAY7SEG(signed char a){
  PORTD = TABLE7SEG[a];
};

void setup() {
  DDRD = 0xFF;
  DDRC = 0x00;
  DISPLAY7SEG(TURN_OF_LED);
  ADMUX = 0x45;
  ADCSRA = 0x87;
  DECODED = 0;
};

void loop() {
  
  counter = 0;
  prevADC = 0;
  do
  {
    adcVal = ADC_read(ADC5);
    Serial.println(analogRead(adcVal));
    if((abs(adcVal-prevADC)<6)&&(adcVal>33))
      counter ++;
    else
      counter = 0;
    prevADC = adcVal;
  }while (counter <20 );
  if((adcVal>33)&&(adcVal<65)) DECODED =1;
  else if ((adcVal>71)&&(adcVal<103)) DECODED = 2;
  else if ((adcVal>114)&&(adcVal<146)) DECODED = 3;
  else if ((adcVal>181)&&(adcVal<213)) DECODED = 4;
  else if ((adcVal>294)&&(adcVal<326)) DECODED = 5;
  else if ((adcVal>396)&&(adcVal<428)) DECODED = 6;
  else if ((adcVal>499)&&(adcVal<581)) DECODED = 7;
  else if ((adcVal>678)&&(adcVal<710)) DECODED = 8;
  else if ((adcVal>767)&&(adcVal<799)) DECODED = 9;
  else if ((adcVal>810)&&(adcVal<884)) DECODED = 10;
  else if ((adcVal>901)&&(adcVal<936)) DECODED = 0;
  else if ((adcVal>946)&&(adcVal<1023)) DECODED = 11;
  do
  {
    adcVal = ADC_read(ADC5);
  }while(adcVal > 33);
  DISPLAY7SEG(DECODED);
};