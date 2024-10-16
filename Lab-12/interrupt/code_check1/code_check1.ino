#include <Arduino.h>

const int soundSensorPin = 5;
const int relayPin = 2;
bool lightState = false; // start 0 (Led ดับ)
volatile bool clapDetected = false; // ตรวจจับการตบมือ

void soundSensorInterrupt() {
  clapDetected = true;
}

void setup() {
  pinMode(soundSensorPin, INPUT); // กำหนดขาของเซ็นเซอร์เสียงเป็นขาเข้า
  pinMode(relayPin, OUTPUT); // กำหนดขาของรีเลย์เป็นขาออก
  digitalWrite(relayPin, LOW); // เริ่มต้นให้หลอดไฟดับ
  Serial.begin(9600); // เริ่มต้นการสื่อสาร Serial ด้วยความเร็ว 9600 บอด
  
  // กำหนด interrupt ให้กับ soundSensorPin
  attachInterrupt(digitalPinToInterrupt(soundSensorPin), soundSensorInterrupt, FALLING);
}

void loop() {
  if (clapDetected) {
    clapDetected = false; // รีเซ็ตสถานะการตรวจจับเสียงปรบมือ
    
    if (lightState == false) {
      digitalWrite(relayPin, HIGH); // เปิดหลอดไฟ
      lightState = true; // เปลี่ยนสถานะของหลอดไฟเป็นเปิด
    } else {
      digitalWrite(relayPin, LOW); // ปิดหลอดไฟ
      lightState = false; // เปลี่ยนสถานะของหลอดไฟเป็นปิด
    }
    delay(500); // หน่วงเวลาเพื่อดีบาวน์เซ็นเซอร์เสียง
  }
}
