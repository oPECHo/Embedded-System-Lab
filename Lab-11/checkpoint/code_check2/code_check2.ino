const int strobe = 7;
const int clock = 9;
const int data = 8;

// Global variables for timekeeping
volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

// Function to send command to TM1638
void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

// Function to reset TM1638
void reset() {
  sendCommand(0x40); // Set auto-increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // Set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00); // Clear display
  }
  digitalWrite(strobe, HIGH);
}

// Function to display a digit on TM1638
void displayDigit(uint8_t digit, uint8_t position) {
  sendCommand(0x44); // Set single address mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0 + (position << 1));
  shiftOut(data, clock, LSBFIRST, digit);
  digitalWrite(strobe, HIGH);
}

// Function to display time on TM1638
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

  sendCommand(0x8F);  // Activate and set brightness
  reset();

  // Configure Timer1
  noInterrupts();           // Disable all interrupts
  TCCR1A = 0;              // Clear Timer1 Control Register A
  TCCR1B = 0;              // Clear Timer1 Control Register B
  TCNT1  = 0;              // Initialize counter value to 0
  OCR1A = 15624;           // Set compare match register for 1Hz increments
  TCCR1B |= (1 << WGM12); // Configure Timer1 for CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  interrupts();            // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  // Timer1 interrupt service routine
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
  displayTime(hours, minutes, seconds);
}

void loop() {
  // Main loop does nothing, all the work is done in the ISR
}
