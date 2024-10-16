// Define pin connections for TM1638 module
const int strobe = 7;  // Strobe pin
const int clock = 9;   // Clock pin
const int data = 8;    // Data pin

// Variables to store the last state of buttons and the LED state
uint8_t lastButtons = 0;
uint8_t ledState = 0;

void sendCommand(uint8_t value)
{
  digitalWrite(strobe, LOW);          // Pull strobe pin LOW to start command
  shiftOut(data, clock, LSBFIRST, value); // Send command value
  digitalWrite(strobe, HIGH);         // Pull strobe pin HIGH to end command
}

void reset()
{
  sendCommand(0x40); // Set auto-increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // Set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00); // Initialize all positions with 0
  }
  digitalWrite(strobe, HIGH);
}

void setup()
{
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8f);  // Activate the TM1638 module
  reset();            // Reset the TM1638 module
}

uint8_t readTM1638_Buttons()
{
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42); // Request button states

  pinMode(data, INPUT); // Switch data pin to input mode

  // Read the 4 button states
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT); // Switch data pin back to output mode
  digitalWrite(strobe, HIGH);
  return buttons;
}

void setTM1638LED(uint8_t value, uint8_t position)
{
  pinMode(data, OUTPUT);

  sendCommand(0x44); // Set write mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC1 + (position << 1)); // Set address for the LED
  shiftOut(data, clock, LSBFIRST, value); // Write LED state
  digitalWrite(strobe, HIGH);
}

void loop()
{
  uint8_t buttons = readTM1638_Buttons();

  // Check if button S1 is pressed
  if (buttons & 0x01)
  {
    for (uint8_t led = 0x01; led != 0; led <<= 1)
    {
      setTM1638LED(1, __builtin_ctz(led)); // Turn on LEDs one by one
      delay(100); // Delay for visibility
    }
    setTM1638LED(0, __builtin_ctz(0x00)); // Ensure all LEDs are turned off
  }

  // Check if button S8 is pressed
  if (buttons & 0x80)
  {
    for (uint8_t led = 0x80; led != 0; led >>= 1)
    {
      setTM1638LED(1, __builtin_ctz(led)); // Turn on LEDs from the last to the first
      delay(100); // Delay for visibility
    }
    setTM1638LED(0, __builtin_ctz(0x00)); // Ensure all LEDs are turned off
  }

  // Update LEDs based on current button states
  for (uint8_t position = 0; position < 8; position++)
  {
    bool buttonPressed = (buttons & (1 << position)) != 0;
    bool wasButtonPressed = (lastButtons & (1 << position)) != 0;

    if (position > 0 && position < 7 && buttonPressed && !wasButtonPressed)
    {
      ledState ^= (1 << position); // Toggle LED state for buttons S2 to S7
    }

    setTM1638LED((ledState & (1 << position)) ? 1 : 0, position);
  }

  lastButtons = buttons; // Update the last button state
}
