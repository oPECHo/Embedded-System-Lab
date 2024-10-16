#include <avr/io.h>
#include <avr/interrupt.h>

//-------------------------------------
void USART_Transmit(unsigned char data)
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

unsigned char USART_Receive(void)
{
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}

unsigned char TEXT[20] = {"Hello world"};
unsigned char i;

void setup() {
  // put your setup code here, to run once:
  UCSR0A = 0x00;
  UCSR0B = 0x18;
  UCSR0C = 0x06;
  UBRR0H = 0;
  UBRR0L = 103;
  TEXT[11] = 13;
  TEXT[12] = 10;
  for (i = 0; i < 13; i++) {
    USART_Transmit(TEXT[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  i = USART_Receive();
  i += 2;
  USART_Transmit(i);
}