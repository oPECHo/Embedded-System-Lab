#define TCNT1_V 62536
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#define wdt_timeout_2s 7

int switchState = 0;
int ledCount = 0;
const int maxLEDs = 8;

void WDT_enable(uint8_t timeout_v) {
  unsigned char bakSREG;
  uint8_t prescaler;
  prescaler = timeout_v & 0x07;
  prescaler |= (1 << WDE);
  if (timeout_v > 7)
    prescaler |= (1 << WDP3);
  bakSREG = SREG;
  cli();
  wdt_reset();
  WDTCSR |= ( (1<<WDCE) | (1<<WDE) );
  WDTCSR = prescaler;
  SREG = bakSREG;
}

void display_led(int count) {
  // Turn on LEDs based on count
  uint8_t tmpB = 0;
  uint8_t tmpD = 0;

  switch(count) {
      case 0: tmpB = 0; tmpD = 0; break;
      case 1: tmpD |= (1 << PD6); break; // Turn on PD6
      case 2: tmpD |= ((1 << PD7) | (1 << PD6)); break; // Turn on PD7
      case 3: tmpB |= 0b00000001; tmpD |= ((1 << PD7) | (1 << PD6)); break;
      case 4: tmpB |= 0b00000011; tmpD |= ((1 << PD7) | (1 << PD6)); break; 
      case 5: tmpB |= 0b00000111; tmpD |= ((1 << PD7) | (1 << PD6)); break;
      case 6: tmpB |= 0b00001111; tmpD |= ((1 << PD7) | (1 << PD6)); break;
      case 7: tmpB |= 0b00011111; tmpD |= ((1 << PD7) | (1 << PD6)); break; 
      case 8: tmpB |= 0b00111111; tmpD |= ((1 << PD7) | (1 << PD6)); break; 
  }
  PORTB = ~tmpB;
  PORTD = ~tmpD;
}

void wait_until_sw_pressed() {
  do {
    do {
    } while (PIND & (1 << PD2)); // Wait until button is pressed (logic low)
    _delay_ms(10); // Debounce delay
  } while (PIND & (1 << PD2)); // Confirm button press

  do {
  } while (!(PIND & (1 << PD2))); // Wait until button is released
  switchState = 1; // Update switch state when button is pressed
}

int main(void) {
  DDRB = 0xFF; // Set PB0-PB5 as output
  DDRD |= (1 << PD7) | (1 << PD6); // Set PD7 and PD6 as output
  DDRD &= ~(1 << PD2); // Set PD2 as input
  PORTD |= (1 << PD2); // Enable internal pull-up, set initial switch state to high

  WDT_enable(wdt_timeout_2s); // Set Watchdog Timer to 2 seconds

  // Ensure all LEDs are off initially
  PORTB = 0xFF;
  PORTD &= ~((1 << PD7) | (1 << PD6));

  while (1) {
    display_led(ledCount);
    wait_until_sw_pressed();

    if (switchState) {
      switchState = 0;
      if (ledCount < maxLEDs) {
        ledCount++;
      } else {
        ledCount = 0;
      }
    }
    wdt_reset(); // Reset the watchdog timer
  }
}
