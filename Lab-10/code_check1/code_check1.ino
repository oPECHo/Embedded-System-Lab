#include <LiquidCrystal.h>

// Initialize the LCD with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2); // Set up the LCD's number of columns and rows
  lcd.clear();      // Clear the display
}

void loop() {
  // Read the value from the potentiometer
  int potValue = analogRead(A0);

  // Convert the analog value to a voltage
  float voltage = potValue * (5.0 / 1023.0);

  // Display the potentiometer value
  lcd.setCursor(0, 0);   // Set the cursor to column 0, row 0
  lcd.print("Display Analog"); 

  // Display the voltage value
  lcd.setCursor(0, 1);   // Set the cursor to column 0, row 1
  lcd.print("VPoten = "); 
  lcd.print(voltage);
  lcd.print(" V");

  delay(500); // Update the display every 500 milliseconds
}
