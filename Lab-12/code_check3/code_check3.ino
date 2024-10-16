#include <DHT.h>
#include <DHT_U.h>

#include <LiquidCrystal.h>
#include <DHT.h>                       // ไฟล์ไลบรารี DHT

#define DHTPIN 2                       // พินที่เชื่อมต่อกับ DATA ของ DHT11
#define DHTTYPE DHT11                  // ชนิดของเซ็นเซอร์ DHT

DHT dht(DHTPIN, DHTTYPE);              // สร้างอ็อบเจ็กต์ DHT
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // RS, E, D4, D5, D6, D7

void setup() {
  lcd.begin(16, 2);                    // เริ่มต้น LCD
  dht.begin();                         // เริ่มต้นเซ็นเซอร์ DHT
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  // อ่านค่าความชื้นและอุณหภูมิ
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // ตรวจสอบว่าอ่านค่าได้สำเร็จหรือไม่
  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor");
    return;
  }

  // แสดงผลลัพธ์ที่ LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(2000);  // รอ 2 วินาทีเพื่ออ่านค่าใหม่
}