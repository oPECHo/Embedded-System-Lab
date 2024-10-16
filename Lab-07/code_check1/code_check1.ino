#include <avr/io.h>
#include <avr/interrupt.h>

//-------------------------------------
void USART_Transmit(unsigned char data)
{
  while (!(UCSR0A & (1 << UDRE0))); // buffer empty
  UDR0 = data;
}

unsigned char USART_Receive(void)
{
  while (!(UCSR0A & (1 << RXC0)));  // Receive
  return UDR0;
}

unsigned char TEXT[] = "My name is Nattanon Chumrit. My ID is 6510110140.";
unsigned char i;

void setup() {
  // put your setup code here, to run once:
  UCSR0A = 0x00;
  UCSR0B = 0x18; // Enable Transmit and Receive
  UCSR0C = 0x06; // USBS = 0 (stop 1 bit), UCSZ01 = 1 , UCSZ00 = 1 data 8 bit
  UBRR0H = 0;
  UBRR0L = 103;  // baud 9600
  TEXT[50] = 13; // ขึ้นบรรทัดใหม่
  TEXT[49] = 10; // 
  for (i = 0; i < sizeof(TEXT); i++) {
    USART_Transmit(TEXT[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  i = USART_Receive();
  i += 2;
  USART_Transmit(i);
}
