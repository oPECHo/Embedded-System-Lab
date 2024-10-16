const int strobe = 7;
const int clock = 9;
const int data = 8;

// Global variables for timekeeping
volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

volatile bool blinkDash = false; // Flag to toggle the '-' symbol
volatile bool showDigit = true;  // Flag to toggle digit blinking

unsigned long lastBlinkTime = 0; // Variable to track the last blink time
const unsigned long blinkInterval = 250; // Interval for blinking (0.5 seconds)

unsigned long lastButtonPress = 0;
bool buttonPressed = false;
uint8_t blinkingDigit = 0; // Position of the digit to blink

// Function to send command to TM1638
void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

// Function to read buttons from TM1638
uint8_t readTM1638_Buttons(void) {
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);

  for (uint8_t i = 0; i < 4; i++) {
    buttons |= shiftIn(data, clock, LSBFIRST) << i;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
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

  uint8_t h1 = digitPatterns[hours / 10];
  uint8_t h2 = digitPatterns[hours % 10];
  uint8_t m1 = digitPatterns[minutes / 10];
  uint8_t m2 = digitPatterns[minutes % 10];
  uint8_t s1 = digitPatterns[seconds / 10];
  uint8_t s2 = digitPatterns[seconds % 10];

  // Display the time with optional blinking
  if (blinkingDigit == 0) {
    displayDigit(showDigit ? h1 : 0x00, 0);
  } else {
    displayDigit(h1, 0);
  }

  if (blinkingDigit == 1) {
    displayDigit(showDigit ? h2 : 0x00, 1);
  } else {
    displayDigit(h2, 1);
  }

  displayDigit(blinkDash ? 0x40 : 0x00, 2); // Display '-' between hours and minutes

  if (blinkingDigit == 3) {
    displayDigit(showDigit ? m1 : 0x00, 3);
  } else {
    displayDigit(m1, 3);
  }

  if (blinkingDigit == 4) {
    displayDigit(showDigit ? m2 : 0x00, 4);
  } else {
    displayDigit(m2, 4);
  }

  displayDigit(blinkDash ? 0x40 : 0x00, 5); // Display '-' between minutes and seconds

  if (blinkingDigit == 6) {
    displayDigit(showDigit ? s1 : 0x00, 6);
  } else {
    displayDigit(s1, 6);
  }

  if (blinkingDigit == 7) {
    displayDigit(showDigit ? s2 : 0x00, 7);
  } else {
    displayDigit(s2, 7);
  }
}

void setup() {
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8F);  // Activate and set brightness
  reset();

  // Configure Timer1 for 1Hz increments (1 second)
  noInterrupts();           // Disable all interrupts
  TCCR1A = 0;              // Clear Timer1 Control Register A
  TCCR1B = 0;              // Clear Timer1 Control Register B
  TCNT1  = 0;              // Initialize counter value to 0
  OCR1A = 15624;           // Set compare match register
  TCCR1B |= (1 << WGM12);  // Configure Timer1 for CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  interrupts();            // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  // Timer1 interrupt service routine for timekeeping
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
  blinkDash = !blinkDash; // Toggle the '-' symbol
  // No need to call displayTime() here
}

void loop() {
  static unsigned long lastButtonDebounceTime = 0;
  uint8_t buttons = readTM1638_Buttons();
  unsigned long currentTime = millis();

  // Handle button presses
  if (buttons & 0x01) { // Check if S1 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      hours = (hours + 1) % 24;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 0;
      lastButtonDebounceTime = currentTime;
    }
  }
  if (buttons & 0x02) { // Check if S2 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      hours = (hours == 0) ? 23 : hours - 1;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 1;
      lastButtonDebounceTime = currentTime;
    }
  }
  if (buttons & 0x08) { // Check if S4 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      minutes = (minutes + 1) % 60;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 3;
      lastButtonDebounceTime = currentTime;
    }
  }
  if (buttons & 0x10) { // Check if S5 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      minutes = (minutes == 0) ? 59 : minutes - 1;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 4;
      lastButtonDebounceTime = currentTime;
    }
  }
  if (buttons & 0x40) { // Check if S7 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      seconds = 59;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 6;
      lastButtonDebounceTime = currentTime;
    }
  }
  if (buttons & 0x80) { // Check if S8 is pressed
    if (currentTime - lastButtonDebounceTime > 200) { // Debounce delay
      seconds = 0;
      lastButtonPress = currentTime;
      buttonPressed = true;
      blinkingDigit = 7;
      lastButtonDebounceTime = currentTime;
    }
  }

  // Handle blinking of digits
  if (buttonPressed && (currentTime - lastButtonPress < 3000)) {
    if (currentTime - lastBlinkTime >= blinkInterval) {
      showDigit = !showDigit; // Toggle the digit blinking
      lastBlinkTime = currentTime;
    }
  } else {
    buttonPressed = false;  // Stop blinking digit after 3 seconds
    showDigit = true;       // Ensure the digit is shown
  }

  displayTime(hours, minutes, seconds); // Update display
}
