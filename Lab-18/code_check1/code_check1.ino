#define Y_LED 0   // Yellow LED connected to GPIO0
#define R_LED 15  // Red LED connected to GPIO15
#define G_LED 2   // Green LED connected to GPIO2
#define DIP12 12  // DIP switch bit 5 connected to GPIO12
#define DIP13 13  // DIP switch bit 6 connected to GPIO13
#define DIP5 5    // DIP switch bit 7 connected to GPIO5
#define DIP16 16  // DIP switch bit 8 connected to GPIO16

// Timing variables
unsigned long prevTime = 0;
bool ledState = false;

void setup() {
  // Initialize LED pins as output
  pinMode(Y_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);

  // Initialize DIP switch pins as input
  pinMode(DIP12, INPUT);
  pinMode(DIP13, INPUT);
  pinMode(DIP5, INPUT);
  pinMode(DIP16, INPUT);
}

void loop() {
  uint8_t mode = getDIPSwitchMode();

  if (digitalRead(DIP16)) {
    // Blink all LEDs if DIP16 is pressed
    blinkRedLED();
  } else {
    // Control LEDs based on the mode value
    setLEDsBasedOnMode(mode);
  }
}

// Function to get mode based on DIP switches
uint8_t getDIPSwitchMode() {
  return (digitalRead(DIP12) * 1) + (digitalRead(DIP13) * 2) + (digitalRead(DIP5) * 4) + (digitalRead(DIP16) * 8);
}

// Function to blink all LEDs
void blinkAllLEDs() {
  if (millis() - prevTime > 1000) {
    prevTime = millis();
    ledState = !ledState;
  }
  digitalWrite(Y_LED, ledState);
  digitalWrite(R_LED, ledState);
  digitalWrite(G_LED, ledState);
}

// Function to set LEDs based on the mode
void setLEDsBasedOnMode(uint8_t mode) {
  // Turn off all LEDs by default
  digitalWrite(Y_LED, LOW);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);

  switch (mode) {
    case 0:
      // All LEDs are off (handled by default)
      break;
    case 1:
      digitalWrite(R_LED, HIGH);  // Red LED on
      digitalWrite(Y_LED, HIGH);  // Red LED on
      break;
    case 2:
      digitalWrite(R_LED, HIGH);  // Red LED on
      digitalWrite(G_LED, HIGH);  // Green LED on
      break;
    case 3:
      digitalWrite(Y_LED, HIGH);  // Yellow LED on
      digitalWrite(G_LED, HIGH);  // Green LED on
      break;
    case 4:
      // Blink the yellow LED every 1000 milliseconds
      blinkAllLEDs();
      break;
    default:
      // Handle unexpected mode values
      break;
  }
}

// Function to blink the yellow LED
void blinkRedLED() {
  if (millis() - prevTime > 1000) {
    prevTime = millis();
    ledState = !ledState;
  }
  digitalWrite(R_LED, ledState);
}
