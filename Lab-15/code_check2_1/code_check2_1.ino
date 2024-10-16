#include <avr/io.h>      // สำหรับการเข้าถึงรีจิสเตอร์ของ AVR
#include <util/delay.h>  // สำหรับการหน่วงเวลา

#define INT0_PIN PD2  // PIN 2 บน PORTD (INT0)
#define INT1_PIN PD3  // PIN 3 บน PORTD (INT1)

// ฟังก์ชันสำหรับการตั้งค่า PORT และ LED
void setup() {
  // ตั้งค่า PORTD และ PORTB เป็นเอาต์พุต
  DDRD = 0xF0;  // 4 บิตสูงของ PORTD เป็นเอาต์พุต, 4 บิตต่ำเป็นอินพุต
  DDRB = 0x0F;  // 4 บิตต่ำของ PORTB เป็นเอาต์พุต, 4 บิตสูงเป็นอินพุต

  // ตั้งค่าพอร์ตให้มีระดับสูง (LED ดับทุกดวง)
  PORTD = 0xFF;
  PORTB = 0xFF;
}

// ฟังก์ชันเพื่อทำให้ไฟวิ่งจากขอบนอกเข้าสู่ตรงกลาง
void led_run_in() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      PORTD = ~(0x10 << j);  // กำหนด PORTD ให้มีบิตที่ตรงกันกับ LED ที่เปิด
      PORTB = ~(0x08 >> j);  // กำหนด PORTB ให้มีบิตที่ตรงกันกับ LED ที่เปิด
      delay(100);        // หน่วงเวลา 100 ms
    }
  }
  // ดับทุก LED
  PORTD = 0xFF;
  PORTB = 0xFF;
}

// ฟังก์ชันเพื่อทำให้ไฟวิ่งจากตรงกลางออกสู่ขอบนอก
void led_run_out() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      PORTD = ~(0x80 >> j);  // กำหนด PORTD ให้มีบิตที่ตรงกันกับ LED ที่เปิด
      PORTB = ~(0x01 << j);  // กำหนด PORTB ให้มีบิตที่ตรงกันกับ LED ที่เปิด
      delay(100);        // หน่วงเวลา 100 ms
    }
  }
  // ดับทุก LED
  PORTD = 0xFF;
  PORTB = 0xFF;
}

void loop() {
  // ตรวจสอบการกดปุ่ม S1 และ S2 โดยการอ่านค่ารีจิสเตอร์
  if (!(PIND & (1 << INT0_PIN)) || !(PIND & (1 << INT1_PIN))) {
    delay(10);  // หน่วงเวลาเพื่อป้องกันการกระตุ้นซ้ำ (debouncing)

    // ตรวจสอบการกดปุ่ม S1
    if (!(PIND & (1 << INT0_PIN))) {
      led_run_in();  // เรียกใช้ฟังก์ชัน led_run_in
    }
    // ตรวจสอบการกดปุ่ม S2
    if (!(PIND & (1 << INT1_PIN))) {
      led_run_out();  // เรียกใช้ฟังก์ชัน led_run_out
    }
  }
}
