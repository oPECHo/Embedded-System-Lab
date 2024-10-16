#include <LiquidCrystal.h>
#define echoPin 7
#define trigPin 6

// Initialize the LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

int maximumRange = 200;
int minimumRange = 0;
long duration, distance;

void setup() {
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  
  // Set the trigPin as an output and the echoPin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clear the previous readings
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by sending a HIGH pulse of 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse)
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (duration / 2 because the pulse travels to the object and back)
  distance = duration / 58.2;
  lcd.clear();
  if (distance > maximumRange) {
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");
    lcd.setCursor(0, 1);
    lcd.print("Out of Range");
  } else if (distance > 5 && distance <= maximumRange) {
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");
    lcd.setCursor(0, 1);
    lcd.print("Range OK");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");
    lcd.setCursor(0, 1);
    lcd.print("TOO CLOSE !");
  }
  delay(1000);
}
