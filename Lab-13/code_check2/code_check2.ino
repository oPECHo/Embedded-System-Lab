#include <avr/io.h>           
#include <avr/interrupt.h>    
#include "my_twi.h"           
#include "my_pcf8574.h"       

uint8_t mode;                
unsigned long last_time;
unsigned long previous_time; // Declare variable for 1s timer

uint8_t prepare_data(uint8_t d)
{
  uint8_t tmp;
  tmp = ~d;                   // Invert bits for Common Anode LED
  tmp |= 0x80;                // Set MSB as input, do not affect LED drive code
  return tmp;                 
}

void setup()
{
  last_time = 0;  
  Serial.begin(38400);
  uint8_t tmp, i;             // Declare 8-bit variables tmp and i
  init_twi_module();          // Initialize TWI module
  mode = 0;                   // Set initial mode to 0
}

void loop(){
  uint8_t tmp;

  tmp = PCF8574_read(0);      // Read from PCF8574
  tmp &= 0x80;                // Mask relevant bit
  if(!tmp)                    // Check if button is pressed
  {
    if( millis() - last_time > 20) // Debounce check
    {
      last_time = millis();   // Update last_time
      tmp = PCF8574_read(0);  // Read again
      tmp &= 0x80;            // Mask relevant bit
      if (!tmp)               // Check if button is still pressed
      {
        Serial.print("Mode: ");
        Serial.println(mode);
        mode < 3 ? mode++ : mode = 0; // Update mode
      }
      do
      {
        tmp = PCF8574_read(0);    // Continuously read
        tmp &= 0x80;              // Mask relevant bit
      }
      while(!tmp);                // Wait for button release
      previous_time = millis();   // Update previous_time
    }
  }
  
  switch(mode){                   // Set switch for 4 modes
    case 0 : count(); break;      // Mode M1: Increment LED
    case 1 : shiftRight(); break; // Mode M2: Shift bits right
    case 2 : shiftLeft(); break;  // Mode M3: Shift bits left
    case 3 : led_blink(); break;  // Mode M4: Blink LED
  }
}

void count(){                     // Mode M1: Increment LED
  static uint8_t count;           // Declare static counter
  uint8_t tmp;
  if((millis() - previous_time) > 1000){ // Check 1 second interval
    previous_time = millis();     // Update previous_time
    count < 127 ? count++ : count = 0; // Increment or reset counter
  }
  tmp = prepare_data(count);      // Prepare LED data
  PCF8574_write(0, tmp);          // Write to PCF8574
}

void shiftRight(){                // Mode M2: Shift bits right
  static uint8_t position;    
  static uint8_t led = 0x40;      // LED starts from left and shifts right
  uint8_t tmp;
  if((millis() - previous_time) > 1000){ // Check 1 second interval
    previous_time = millis();     // Update previous_time
    position < 6 ? position++ : position = 0; // Update position
    led = 0x40 >> position;       // Shift bit right
  }
  tmp = prepare_data(led);        // Prepare LED data
  PCF8574_write(0, tmp);          // Write to PCF8574
}

void shiftLeft(){                 // Mode M3: Shift bits left
  static uint8_t position;
  static uint8_t led = 0x01;      // LED starts from right and shifts left
  uint8_t tmp;
  if((millis() - previous_time) > 1000){ // Check 1 second interval
    previous_time = millis();     // Update previous_time
    position < 6 ? position++ : position = 0; // Update position
    led = 0x01 << position;       // Shift bit left
  }
  tmp = prepare_data(led);        // Prepare LED data
  PCF8574_write(0, tmp);          // Write to PCF8574
}

void led_blink(){                 // Mode M4: Blink LED
  static uint8_t led = 0xFF;      // LED starts ON
  uint8_t tmp;
  if((millis() - previous_time) > 1000){ // Check 1 second interval
    previous_time = millis();     // Update previous_time
    led = ~led;                   // Toggle LED state
  }
  tmp = prepare_data(led);        // Prepare LED data
  PCF8574_write(0, tmp);          // Write to PCF8574
}
