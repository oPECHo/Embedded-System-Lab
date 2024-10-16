#include <LiquidCrystal.h>

// Initialize the LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Make some custom characters
byte box0[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b00110,
  0b00111,
  0b00111,
};

byte box1[8] = {
  0b00011,
  0b00001,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

byte box2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10001,
};

byte box3[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01111,
  0b00110,
  0b00010,
  0b00011,
};

byte box4[8] = {
  0b00111,
  0b01101,
  0b01111,
  0b01111,
  0b01111,
  0b01111,
  0b11111,
  0b11111,
};

byte box5[8] = {
  0b11111,
  0b11110,
  0b11100,
  0b11000,
  0b01000,
  0b01100,
  0b00000,
  0b00000,
};

byte box6[8] = {
  0b11110,
  0b11111,
  0b11111,
  0b10000,
  0b11110,
  0b00000,
  0b00000,
  0b00000,
};

byte box7[8] = {
  0b10000,
  0b10000,
  0b00100,
  0b00101,
  0b10101,
  0b10101,
  0b01110,
  0b00100,
};

void setup() 
{
  // Initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);

  // Create custom characters
  lcd.createChar(0, box0);
  lcd.createChar(1, box1);
  lcd.createChar(2, box2);
  lcd.createChar(3, box3);
  lcd.createChar(4, box4);
  lcd.createChar(5, box5);
  lcd.createChar(6, box6);
  lcd.createChar(7, box7);

  // Clears the LCD screen
  lcd.clear();
}

void loop() 
{
  for (int position = 0; position < 13; position++) 
  {
    // Clear the LCD before printing new characters
    lcd.clear();

    // Print all the custom characters
    lcd.setCursor(position, 0);
    lcd.write(byte(0));

    lcd.setCursor(position, 1);
    lcd.write(byte(1));

    lcd.setCursor(position + 1, 0);
    lcd.write(byte(2));

    lcd.setCursor(position + 1, 1);
    lcd.write(byte(3));

    lcd.setCursor(position + 2, 0);
    lcd.write(byte(4));

    lcd.setCursor(position + 2, 1);
    lcd.write(byte(5));

    lcd.setCursor(position + 3, 0);
    lcd.write(byte(6));

    lcd.setCursor(position + 3, 1);
    lcd.write(byte(7));

    // lcd.setCursor(8, 0);
    // lcd.print("Custom");

    // lcd.setCursor(6, 1);
    // lcd.print("Characters");

    // Delay to allow viewing the display before it updates
    delay(500);
  }
}
