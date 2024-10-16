#include <avr/io.h>           // I/O operations
#include <avr/interrupt.h>    // Interrupt handling
#include "my_twi.h"           // TWI (I2C) functions
#include "my_pcf8574.h"       // PCF8574 functions

uint8_t count;                // Button press counter
unsigned long last_time;      // Last button press time

// Prepare data for PCF8574
uint8_t prepare_data(uint8_t d)
{
  uint8_t tmp;
  tmp = ~d;                   // Invert bits for Common Anode LEDs
  tmp |= 0x80;                // Set MSB for LED control
  return tmp;
}

// Setup routine
void setup()
{
  last_time = 0;              // Initialize time
  Serial.begin(38400);        // Start serial
  uint8_t tmp, i;             
  init_twi_module();          // Initialize TWI
  count = 0;                  // Initialize counter
  tmp = prepare_data(count);  // Prepare initial data
  PCF8574_write(0, tmp);      // Write data to PCF8574
}

void loop()
{
  uint8_t tmp;

  tmp = PCF8574_read(0);      // Read PCF8574
  tmp &= 0x80;                // Mask relevant bit
  if (!tmp)                   // Check if button pressed
  {
    if (millis() - last_time > 20) // Debounce check
    {
      last_time = millis();   // Update time
      tmp = PCF8574_read(0);  // Read again
      tmp &= 0x80;            // Mask relevant bit
      if (!tmp)               // If button still pressed
      {
        Serial.println("Keypressed detected"); // Notify
        count <= 127 ? count++ : count = 0; // Update count
      }
      do
      {
        tmp = PCF8574_read(0);  // Continuously read
        tmp &= 0x80;            // Mask relevant bit
      }
      while (!tmp);            // Wait for button release
    }
  }
  tmp = prepare_data(count);  // Prepare LED data
  PCF8574_write(0, tmp);      // Update PCF8574
}
