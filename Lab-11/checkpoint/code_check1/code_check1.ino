const int strobe = 7;
const int clock = 9;
const int data = 8;

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

void reset() {
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00); // clear display
  }
  digitalWrite(strobe, HIGH);
}

void displayDigit(uint8_t digit, uint8_t position) {
  // Set the address for the given position
  sendCommand(0x44); // set single address mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0 + (position << 1));
  shiftOut(data, clock, LSBFIRST, digit);
  digitalWrite(strobe, HIGH);
}

void displayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  // Digit patterns for 7-segment display
  uint8_t digitPatterns[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
  };
  
  // Extract each digit from hours, minutes, and seconds
  uint8_t h1 = digitPatterns[hours / 10];
  uint8_t h2 = digitPatterns[hours % 10];
  uint8_t m1 = digitPatterns[minutes / 10];
  uint8_t m2 = digitPatterns[minutes % 10];
  uint8_t s1 = digitPatterns[seconds / 10];
  uint8_t s2 = digitPatterns[seconds % 10];
  
  // Display hours, minutes, and seconds with '-' between them
  displayDigit(h1, 0);  // 1st digit of hours
  displayDigit(h2, 1);  // 2nd digit of hours
  displayDigit(0x40, 2);  // '-' between hours and minutes
  displayDigit(m1, 3);  // 1st digit of minutes
  displayDigit(m2, 4);  // 2nd digit of minutes
  displayDigit(0x40, 5);  // '-' between minutes and seconds
  displayDigit(s1, 6);  // 1st digit of seconds
  displayDigit(s2, 7);  // 2nd digit of seconds
}

void setup() {
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8F);  // activate and set brightness
  reset();
}

void loop() {
  static uint8_t hours = 0;
  static uint8_t minutes = 0;
  static uint8_t seconds = 0;

  // Display current time
  displayTime(hours, minutes, seconds);

  // Increment time
  delay(1000);  // wait for 1 second
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}
