#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x20  //กำหนด Address ของ I2C

const byte ROWS = 4;  //กำหนดจำนวนของ Rows
const byte COLS = 4;  //กำหนดจำนวนของ Columns

//กำหนด Key ที่ใช้งาน (4x4)
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

//กำหนด Pin ที่ใช้งาน (4x4)
byte rowPins[ROWS] = { 0, 1, 2, 3 };  //เชื่อมต่อกับ Pin แถวของปุ่มกด
byte colPins[COLS] = { 4, 5, 6, 7 };  //เชื่อมต่อกับ Pin คอลัมน์ของปุ่มกด

//PCF8574 : กำหนดเบอร์ IC
Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574);

void setup() {
  Wire.begin();                    //เรียกการเชื่อมต่อ Wire
  keypad.begin(makeKeymap(keys));  //เรียกการเชื่อมต่อ Keymap
  Serial.begin(9600);
}

void loop() {

  char key = keypad.getKey();  //สร้างตัวแปรชื่อ key ชนิด char เพื่อเก็บตัวอักขระที่กด

  if (key) {  //หากมีการกด keypad
    Serial.println(key);
    Serial.println(" is pressed");
  }
}

void I2C_bus_scan(void) {  //ฟังก์ชั่นการค้นหา byte ที่เก็บค่า address ของอุปกรณ์ I2c
  Serial.println();
  Serial.println("www.9arduino.com ...");
  Serial.println("I2C scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++)  //Loop ค้นหา Address
  {
    Wire.beginTransmission(i);  //เป็นการ transmission
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
