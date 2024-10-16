#define TCNT1_V 62536
#define num_led 12
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
static unsigned char activeLED = 0;
unsigned char LED_Status[num_led];

void WDT_enable(uint8_t timeout_v)
{
  unsigned char bakSREG;
  uint8_t prescaler;
  prescaler = timeout_v & 0x07;
  if(timeout_v > 7)
      prescaler |= (1 << WDP3);
  bakSREG = SREG;
  cli();
  wdt_reset();
  WDTCSR |= ( (1<<WDCE) | (1<<WDE) );
  WDTCSR = prescaler;
  SREG = bakSREG;
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = TCNT1_V;
  display_each_led(activeLED, LED_Status[activeLED]);
  activeLED++;
  if(activeLED >= num_led)
    activeLED = 0;
}

void display_each_led(int led_num, char status_on)
{
  DDRC &= 0b11110000;
  switch(led_num)
  {
    case 0: DDRC |= 0b00011; if(status_on) PORTC = 0b0001; else PORTC = 0b0000; break;
    case 1: DDRC |= 0b00011; if(status_on) PORTC = 0b0010; else PORTC = 0b0000; break;
    case 2: DDRC |= 0b00110; if(status_on) PORTC = 0b0010; else PORTC = 0b0000; break;
    case 3: DDRC |= 0b00110; if(status_on) PORTC = 0b0100; else PORTC = 0b0000; break;
    case 4: DDRC |= 0b01100; if(status_on) PORTC = 0b0100; else PORTC = 0b0000; break;
    case 5: DDRC |= 0b01100; if(status_on) PORTC = 0b1000; else PORTC = 0b0000; break;
    case 6: DDRC |= 0b00101; if(status_on) PORTC = 0b0001; else PORTC = 0b0000; break;
    case 7: DDRC |= 0b00101; if(status_on) PORTC = 0b0100; else PORTC = 0b0000; break;
    case 8: DDRC |= 0b01010; if(status_on) PORTC = 0b0010; else PORTC = 0b0000; break;
    case 9: DDRC |= 0b01010; if(status_on) PORTC = 0b1000; else PORTC = 0b0000; break;
    case 10: DDRC |= 0b01001; if(status_on) PORTC = 0b0001; else PORTC = 0b0000; break;
    case 11: DDRC |= 0b01001; if(status_on) PORTC = 0b1000; else PORTC = 0b0000; break;
  }
}

#define wdt_timeout_125ms 3
#define wdt_timeout_2sec  7
int main(void)
{
  cli();
  TIMSK1 = (1<<TOIE1);  //set timer 1
  TCCR1B = 0x02; // N = 8 Prescaler
  TCCR1A = 0x00; // Timer 1 normal mode
  TCNT1 = TCNT1_V;
  WDT_enable(wdt_timeout_125ms);
  sei();
  int i;
  for(int i = 0; i < num_led ; i++)
      LED_Status[i] = 0;
  while(1)
  {
    for(int i = 0; i < num_led ; i++)
      LED_Status[i] = 1;
    for( i = 0; i < 10; i++ )
    {
      _delay_ms(100);
      wdt_reset();
    }
    for(int i = 0; i < num_led ; i++)
      LED_Status[i] = 0;
    for( i = 0; i < 10; i++ )
    {
      _delay_ms(100);
      wdt_reset();
    }
  }
}