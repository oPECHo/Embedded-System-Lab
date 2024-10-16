#include <avr/sleep.h>      // Include sleep functions
#include "my_wdt.h"         // Include WDT functions
#define INT0_PIN 2          // Define PIN2 for INT0
#define INT1_PIN 3          // Define PIN3 for INT1
const int STROBE_PIN = 14;  // Define PIN14 for TM1638 STROBE_PIN
const int CLOCK_PIN = 15;   // Define PIN15 for TM1638 CLOCK_PIN
const int DATA_PIN = 16;    // Define PIN16 for TM1638 DATA_PIN

uint8_t secondsRemaining = 12; // Seconds remaining
uint16_t subsecondsRemaining = 0; // Subseconds remaining

uint8_t buttons;            // Variable for button state
volatile bool countStarted = false;
volatile bool countFinished = false;
unsigned long lastBlinkTime = 0;
bool countMode = false;
unsigned long startTime, prevTime;

void SLEEP_DISABLE()  // Function to disable sleep mode
{                     
  SMCR &= 0xFE;       // Clear SE bit in SMCR register
}  

void SLEEP_INITIALIZE(uint8_t m)  // Function to initialize sleep mode
{                                 
  SMCR = (m << 1) | 0x01;         // Set sleep mode and enable sleep bit
}  

uint8_t readTM1638_Buttons(void)
// Function to read button states from TM1638
{
  uint8_t buttons = 0;            // Initialize buttons to 0
  digitalWrite(STROBE_PIN, LOW);  // Set STROBE_PIN low
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0x42);

  pinMode(DATA_PIN, INPUT);

  for (uint8_t i = 0; i < 4; i++) {
    uint8_t v = shiftIn(DATA_PIN, CLOCK_PIN, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(STROBE_PIN, HIGH);  // Set STROBE_PIN high
  return buttons;
}

void sendCommand(uint8_t value)
// Function to send commands to TM1638
{
  digitalWrite(STROBE_PIN, LOW);                   // Set STROBE_PIN low
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);  // Send command
  digitalWrite(STROBE_PIN, HIGH);                  // Set STROBE_PIN high to finish sending
}

void reset()
// Function to reset TM1638
{
  sendCommand(0x40);  // Set auto increment mode
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0xc0);  // Set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0x00);
  }
  digitalWrite(STROBE_PIN, HIGH);
}

ISR(WDT_vect) {
  buttons = readTM1638_Buttons();
  if (buttons) {
    WDT__disable();
  }
}

void setup() {
  secondsRemaining = 12;
  subsecondsRemaining = 0;

  pinMode(STROBE_PIN, OUTPUT);  // Set STROBE_PIN as OUTPUT
  pinMode(CLOCK_PIN, OUTPUT);   // Set CLOCK_PIN as OUTPUT
  pinMode(DATA_PIN, OUTPUT);    // Set DATA_PIN as OUTPUT
  startTime = millis();         // Record the start time
  sendCommand(0x8f);            // Activate TM1638
  reset();                      // Reset TM1638
  displayTime();                // Display initial time
  Serial.begin(9600);
}

void loop() {
  uint8_t buttons = readTM1638_Buttons();
  unsigned long currentMillis = millis();
  if ((currentMillis - startTime) > 5000 && !countStarted) {
    // Check if no button press for more than 5s
    reset();  // Reset TM1638 if no button press
    WDT_interrupt_enable(wdt_timeout_2sec);
    SLEEP_INITIALIZE(2);  // Set sleep mode to power-down
    sleep_cpu();          // Put the CPU to sleep
  }

  if ((buttons == 0x80) && (currentMillis - startTime > 200)) {  // If S8 is pressed
    countStarted = true;                                         // Set countStarted to true
  } else if ((buttons == 0x01) && (currentMillis - startTime > 200)) {
    startTime = currentMillis;
    countMode = !countMode;
    secondsRemaining = countMode ? 0 : 12;
    subsecondsRemaining = 0;
    displayTime();
  } else if ((buttons) && (currentMillis - startTime > 200)) {
    countStarted = false;
    startTime = currentMillis;
    secondsRemaining = countMode ? 0 : 12;
    subsecondsRemaining = 0;
    displayTime();
  }

  if (countStarted) {                      // Check if counting has started
    if ((currentMillis - prevTime) > 10) {  // Update every 10 milliseconds
      prevTime = currentMillis;
      if ((secondsRemaining == 0 && subsecondsRemaining == 0 && !countMode) || 
          (secondsRemaining == 12 && subsecondsRemaining == 0 && countMode)) {
        reset();
        delay(300);
        displayTime();
        delay(300);
        reset();
        delay(300);
        displayTime();
        delay(300);
        reset();
        countStarted = false;
        WDT_interrupt_enable(wdt_timeout_2sec);
        SLEEP_INITIALIZE(2);
        sleep_cpu();
      } else {
        if (!countMode) {
          if (subsecondsRemaining > 0) {
            subsecondsRemaining--;  // Decrease subseconds
          } else if (secondsRemaining > 0) {
            secondsRemaining--;
            subsecondsRemaining = 99;  // Reset subseconds
          } else {
            subsecondsRemaining = 0;
            countFinished = true;
          }
        } else {
          if (subsecondsRemaining < 99) {
            subsecondsRemaining++;  // Increase subseconds
          } else if (secondsRemaining < 12) {
            secondsRemaining++;
            subsecondsRemaining = 0;  // Reset subseconds
          } else {
            subsecondsRemaining = 99;
            countFinished = true;
          }
        }
        displayTime();
      }
    }
  }
}

void displayDigit(uint8_t digit, uint8_t position) {
  sendCommand(0x44); // Set single address mode
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0xc0 + (position << 1));
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digit);
  digitalWrite(STROBE_PIN, HIGH);
}

void displayTime()  // Function to display time on 7-segment display
{
  const uint8_t digitPatterns[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
  };

  uint8_t seconds = secondsRemaining;
  uint8_t hundredths = subsecondsRemaining;

  uint8_t s1 = digitPatterns[seconds / 10];
  uint8_t s2 = digitPatterns[seconds % 10];
  uint8_t h1 = digitPatterns[hundredths / 10];
  uint8_t h2 = digitPatterns[hundredths % 10];

  uint8_t displayValues[8] = { 0x00, 0x00, 0x00, s1, s2, 0x40, h1, h2 };
  if (!countFinished) {
    displayValues[0] = countMode ? 0x1C : 0x5E;
    displayValues[1] = countMode ? 0x73 : 0x54;
  }

  sendCommand(0x44);  // Set single address mode
  for (uint8_t i = 0; i < 8; i++) {
    displayDigit(displayValues[i], i);
  }
}
