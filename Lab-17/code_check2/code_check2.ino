#include "my_EEPROM.h"
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x20

const int strobe = 4;
const int clock = 3;
const int data = 2;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 7, 6, 5, 4 };
byte colPins[COLS] = { 3, 2, 1, 0 };

Keypad_I2C keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

void I2C_bus_scan(void);
void sendCommand(uint8_t value);
void reset(void);
uint8_t readTM1638_Buttons(void);

void setup() {
  Serial.begin(38400);
  // display_all_data_in_EEPROM();

  Wire.begin();
  keypad.begin();
  I2C_bus_scan();

  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8F);  // activate 7segments and set maximum brightness
  reset();
}

int State = 1;
uint16_t eeprom_address;
uint8_t eeprom_data;
const uint8_t digits[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

int debouce = 150;
bool display_once = false;
int Ch_value_addrs = 0;
int Ch_value_data = 0;

void loop() {
  char key;

  switch (State) {
    case 1:
      reset();
      delay(debouce);
      if (readTM1638_Buttons() == 1) {
        State = 2;
        Serial.println("State: 2");
      } else if (readTM1638_Buttons() == 128) {
        State = 3;
        Serial.println("State: 3");
      }
      break;
    case 2:
      display_all_data_in_EEPROM();
      State = 1;
      break;
    case 3:
      delay(debouce);
      if (readTM1638_Buttons() == 128) {
        State = 4;
        Serial.println("State: 4");
      }
      //  else if (readTM1638_Buttons() == 1) {
      //   State = 5;
      //   Serial.println("State: 5");
      // }
      break;
    case 4:
      delay(debouce);
      if (readTM1638_Buttons() == 128) {
        State = 3;
        Serial.println("State: 3");
      } else if (readTM1638_Buttons() == 1) {
        State = 5;
        Serial.println("State: 5");
      }
      break;
    case 5:
      // EEPROM_Erase_and_Write1B(eeprom_address, eeprom_data);
      State = 1;
      break;
  }
}

void I2C_bus_scan(void) {
  Serial.println();
  Serial.println("www.9arduino.com ...");
  Serial.println("I2C scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1);
    }
  }
  Serial.println("Done.");
  Serial.print("Found ");
  Serial.print(count, DEC);
  Serial.println(" device(s).");
}

uint8_t mapped_keypad(char value) {
  switch (value) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A': return 10;
    case 'B': return 11;
    case 'C': return 12;
    case 'D': return 13;
    case '*': return 14;
    case '#': return 15;
  }
}

void sendCommand(uint8_t value) {
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

void reset(void) {
  sendCommand(0x40);  // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);  // set starting address to 0
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

uint8_t readTM1638_Buttons(void) {
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);

  for (uint8_t i = 0; i < 4; i++) {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
}

void DISP_TM(void) {
  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t adrs_digit00 = eeprom_address % 10;
  uint8_t adrs_digit01 = (eeprom_address / 10) % 10;
  uint8_t adrs_digit02 = (eeprom_address / 100) % 10;
  uint8_t adrs_digit03 = (eeprom_address / 1000) % 10;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit00]);
  digitalWrite(strobe, HIGH);

  uint8_t data_digit00 = eeprom_data & 0x0F;
  uint8_t data_digit01 = (eeprom_data & 0xF0) >> 4;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, digits[data_digit01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, digits[data_digit00]);
  digitalWrite(strobe, HIGH);
}

void DISP_TM_CHANGE_ADDRS_1000(void) {
  // Serial.println("t1");

  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t adrs_digit03 = (eeprom_address / 1000) % 10;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x40);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);
}

void DISP_TM_CHANGE_ADDRS_1100(void) {
  // Serial.println("t2");

  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t adrs_digit02 = (eeprom_address / 100) % 10;
  uint8_t adrs_digit03 = (eeprom_address / 1000) % 10;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  delay(500);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);
}

void DISP_TM_CHANGE_ADDRS_1110(void) {
  // Serial.println("t3");

  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t adrs_digit01 = (eeprom_address / 10) % 10;
  uint8_t adrs_digit02 = (eeprom_address / 100) % 10;
  uint8_t adrs_digit03 = (eeprom_address / 1000) % 10;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit01]);
  digitalWrite(strobe, HIGH);

  delay(500);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit01]);
  digitalWrite(strobe, HIGH);
}

void DISP_TM_CHANGE_ADDRS_1111(void) {
  // Serial.println("t4");

  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t adrs_digit00 = eeprom_address % 10;
  uint8_t adrs_digit01 = (eeprom_address / 10) % 10;
  uint8_t adrs_digit02 = (eeprom_address / 100) % 10;
  uint8_t adrs_digit03 = (eeprom_address / 1000) % 10;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit00]);
  digitalWrite(strobe, HIGH);

  delay(500);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC0);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit03]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC2);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit02]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC4);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC6);
  shiftOut(data, clock, LSBFIRST, digits[adrs_digit00]);
  digitalWrite(strobe, HIGH);
}

void DIPS_TM_ADDRS_SELECTION(uint8_t value) {
  switch (value) {
    case 1:
      DISP_TM_CHANGE_ADDRS_1000();
      break;
    case 2:
      DISP_TM_CHANGE_ADDRS_1100();
      break;
    case 3:
      DISP_TM_CHANGE_ADDRS_1110();
      break;
    case 0:
      DISP_TM_CHANGE_ADDRS_1111();
      break;
  }
}

void DISP_TM_CHANGE_DATA_10(void) {
  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t data_digit_01 = (eeprom_data & 0xF0) >> 4;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, digits[data_digit_01]);
  digitalWrite(strobe, HIGH);

  delay(500);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, digits[data_digit_01]);
  digitalWrite(strobe, HIGH);
}

void DISP_TM_CHANGE_DATA_11(void) {
  pinMode(data, OUTPUT);

  sendCommand(0x44);

  uint8_t data_digit00 = eeprom_data & 0x0F;
  uint8_t data_digit01 = (eeprom_data & 0xF0) >> 4;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, digits[data_digit01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, digits[data_digit00]);
  digitalWrite(strobe, HIGH);

  delay(500);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, 0x00);
  digitalWrite(strobe, HIGH);

  delay(500);

  sendCommand(0x44);

  uint8_t data_digit_00 = eeprom_data & 0x0F;
  uint8_t data_digit_01 = (eeprom_data & 0xF0) >> 4;

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCC);
  shiftOut(data, clock, LSBFIRST, digits[data_digit_01]);
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xCE);
  shiftOut(data, clock, LSBFIRST, digits[data_digit_00]);
  digitalWrite(strobe, HIGH);
}

void DIPS_TM_DATA_SELECTION(uint8_t value) {
  switch (value) {
    case (1): DISP_TM_CHANGE_DATA_10(); break;
    case (0): DISP_TM_CHANGE_DATA_11(); break;
  }
}