#include <avr/io.h>    // สำหรับการเข้าถึงรีจิสเตอร์ของ AVR
#include <avr/interrupt.h> // สำหรับการจัดการการขัดจังหวะ (interrupts)
#include <avr/sleep.h> // สำหรับการจัดการโหมดหลับ (sleep modes)
#include <util/delay.h> // สำหรับการหน่วงเวลา

#define INT0_PIN PD2   // PIN 2 บน PORTD (INT0)
#define INT1_PIN PD3   // PIN 3 บน PORTD (INT1)

// ฟังก์ชันสำหรับปิดการเข้าสู่โหมดหลับ
void SLEEP_DISABLE() {
  SMCR &= ~(1 << SE);  // ปิดบิต SE ในรีจิสเตอร์ SMCR
}

// ฟังก์ชันสำหรับเปิดการเข้าสู่โหมดหลับ
void SLEEP_INITIALIZE(uint8_t m) {
  SMCR = (m << SM0) | (1 << SE); // ตั้งค่าโหมดหลับและเปิดใช้งาน
}

void setup() {
  // ตั้งค่า INT0 และ INT1 เป็นอินพุตพร้อม Pull-up resistor
  DDRD &= ~((1 << INT0_PIN) | (1 << INT1_PIN)); // ตั้งค่า INT0 และ INT1 เป็นอินพุต
  PORTD |= (1 << INT0_PIN) | (1 << INT1_PIN);  // เปิดใช้งาน Pull-up resistors

  // ตั้งค่าการขัดจังหวะ (Interrupt) สำหรับ INT0 และ INT1
  EICRA = (1 << ISC01) | (1 << ISC11);  // ตั้งค่าให้ INT0 และ INT1 ใช้ขอบลง (FALLING EDGE)
  EIMSK = (1 << INT0) | (1 << INT1);   // เปิดใช้งาน INT0 และ INT1

  // ตั้งค่า PORTD และ PORTB เป็นเอาต์พุต
  DDRD = 0xF0;  // ตั้ง PORTD เป็นเอาต์พุต (4 บิตสูง)
  DDRB = 0x0F;  // ตั้ง PORTB เป็นเอาต์พุต (4 บิตต่ำ)
  
  // ดับทุก LED โดยตั้ง PORTD และ PORTB เป็นระดับสูง
  PORTD = 0xFF;
  PORTB = 0xFF;

  sei(); // เปิดใช้งานการขัดจังหวะ (interrupts)
}

void loop() {
  SLEEP_INITIALIZE(SLEEP_MODE_IDLE);  // ตั้งค่าโหมดหลับแบบ Idle
  sleep_cpu();                       // เข้าสู่โหมดหลับ
  SLEEP_DISABLE();                   // เมื่อออกจากโหมดหลับ ให้ปิดการเข้าสู่โหมดหลับ
}

ISR(INT0_vect) {  // ISR สำหรับ INT0
  _delay_ms(15); // ใช้สำหรับ debouncing
  
  if (!(PIND & (1 << INT0_PIN))) { // ตรวจสอบว่าปุ่ม INT0 ถูกกด
    // ไฟวิ่งจากขอบนอกเข้าสู่ตรงกลาง
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        PORTD = ~(0x10 << j); // ขอบนอกของ PORTD
        PORTB = ~(0x08 >> j); // ขอบนอกของ PORTB
        _delay_ms(100);
      }
    }
    PORTD = 0xFF; // ดับทุก LED
    PORTB = 0xFF; // ดับทุก LED
  }
}

ISR(INT1_vect) {  // ISR สำหรับ INT1
  _delay_ms(15); // ใช้สำหรับ debouncing

  if (!(PIND & (1 << INT1_PIN))) { // ตรวจสอบว่าปุ่ม INT1 ถูกกด
    // ไฟวิ่งจากตรงกลางออกสู่ขอบนอก
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        PORTD = ~(0x80 >> j); // ขอบนอกของ PORTD
        PORTB = ~(0x01 << j); // ขอบนอกของ PORTB
        _delay_ms(100);
      }
    }
    PORTD = 0xFF; // ดับทุก LED
    PORTB = 0xFF; // ดับทุก LED
  }
}
