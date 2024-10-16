#include "my_EEPROM.h"

// ฟังก์ชันเพื่อสแกน EEPROM และจัดการข้อมูล
uint16_t scan_EEPROM(void) {
  uint16_t data_location[3];  // กำหนดขนาดของอาเรย์ให้มี 3 ตำแหน่ง
  int i, number_data_found;
  uint8_t d, d0, d1, d2;
  number_data_found = 0;
  Serial.println("Performing EEPROM scanning...");

  // สแกน EEPROM เพื่อหาข้อมูลที่ไม่ใช่ 0xFF
  for (i = 0; i < 1024; i++) {
    d = EEPROM_read1byte(i);
    if (d != 0xFF) {
      if (number_data_found < 3) {  // จำกัดจำนวนตำแหน่งที่เก็บไว้ที่ 3
        data_location[number_data_found] = i;
        number_data_found++;
      }
      Serial.print(d, HEX);  // แสดงข้อมูลในรูปแบบฮีกซาเดซิมอล
      Serial.print(" is found in EEPROM location[");
      Serial.print(i);
      Serial.println("]");
    }
  }

  // การจัดการข้อมูลที่พบ
  if (number_data_found > 3) {
    Serial.println("More than three data are found in EEPROM");
    for (int i = 0; i < number_data_found; i++) {
      EEPROM_Erase_only(data_location[i]);
    }
    Serial.println("EEPROM Clear.");
  } else if (number_data_found == 3) {
    Serial.print("Data locations: ");
    Serial.print(data_location[0]);
    Serial.print(", ");
    Serial.print(data_location[1]);
    Serial.print(", ");
    Serial.println(data_location[2]);

    d0 = EEPROM_read1byte(data_location[0]);  // อ่านค่าปัจจุบันจากตำแหน่งที่พบ
    d1 = EEPROM_read1byte(data_location[1]);
    d2 = EEPROM_read1byte(data_location[2]);

    if (d2 == 0x0F) {
      EEPROM_Erase_only(data_location[2]);
      if (data_location[2] - 3 >= 0) {  // ตรวจสอบให้แน่ใจว่าตำแหน่งไม่ติดลบ
        EEPROM_Write_to_Empty_location(data_location[2] - 3, 0x0F);
      } else {
        Serial.println("Warning: Attempting to write to an invalid EEPROM location.");
      }
    }
    if (d1 == 0xCE) {
      EEPROM_Erase_only(data_location[1]);
      if (data_location[1] - 3 >= 0) {  // ตรวจสอบให้แน่ใจว่าตำแหน่งไม่ติดลบ
        EEPROM_Write_to_Empty_location(data_location[1] - 3, 0xCE);
      } else {
        Serial.println("Warning: Attempting to write to an invalid EEPROM location.");
      }
    }
    if (d0 == 0xFA) {
      EEPROM_Erase_only(data_location[0]);
      if (data_location[0] - 3 >= 0) {  // ตรวจสอบให้แน่ใจว่าตำแหน่งไม่ติดลบ
        EEPROM_Write_to_Empty_location(data_location[0] - 3, 0xFA);
      } else {
        Serial.println("Warning: Attempting to write to an invalid EEPROM location.");
      }
    }
  } else if (number_data_found == 0) {
    Serial.println("No data found in EEPROM");
    // เขียนข้อมูลเริ่มต้นใหม่
    EEPROM_Write_to_Empty_location(1023, 0x0F);
    EEPROM_Write_to_Empty_location(1022, 0xCE);
    EEPROM_Write_to_Empty_location(1021, 0xFA);
    Serial.println("Default values have been added to EEPROM");
  }
}

// ฟังก์ชัน setup
void setup() {
  Serial.begin(38400);
  display_all_data_in_EEPROM(); // ฟังก์ชันที่แสดงข้อมูลทั้งหมดใน EEPROM
  scan_EEPROM();  // เรียกฟังก์ชันเพื่อสแกนและจัดการข้อมูลใน EEPROM
}

// ฟังก์ชัน loop
void loop() {
  // ฟังก์ชันนี้จะทำงานซ้ำ ๆ แต่ในที่นี้ไม่ได้ทำอะไร
}
