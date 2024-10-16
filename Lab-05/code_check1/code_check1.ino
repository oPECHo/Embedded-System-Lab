#include <avr/io.h> // เรียกโปรแกรม io.h
#include <avr/interrupt.h> // เรียกโปรแกรม interrupt.h

void do_nothing(void) {} // ฟังก์ชันที่ทำอะไรไม่อยู่, ใช้ในการ busy-wait loop

int main(void)
{
  DDRB = 0xFF; // กำหนดให้ทุกพินใน PORTB เป็น output
  DDRD = 0xFF; // กำหนดให้ทุกพินใน PORTD เป็น output

  // กำหนดค่า ADMUX
  // ADMUX[7:6] = 00 -> ใช้ AREF pin เป็น reference voltage
  // ADMUX[5] = 1 -> ADLAR = 1 (ตั้งค่าให้ผลลัพธ์การแปลง ADC ถูกจัดเรียงทางซ้าย)
  // ADMUX[3:0] = 0000 -> เลือกช่อง ADC0
  ADMUX = 0b00100000;

  while (1) // วนลูปไม่มีที่สิ้นสุด
  {
    // กำหนดค่า ADCSRA
    // ADCSRA[7] = 1 -> เปิดใช้งาน ADC
    // ADCSRA[6] = 1 -> เริ่มการแปลงค่า
    // ADCSRA[2:0] = 111 -> ตั้งค่า prescaler เป็น 128
    ADCSRA = 0b11000111;

    // รอให้การแปลง ADC เสร็จสมบูรณ์
    while (!(ADCSRA & (1 << ADIF)))
      do_nothing();

    // อ่านผลลัพธ์การแปลง ADC
    PORTD = ADCH; // ส่งออกไบต์สูงของผลลัพธ์ ADC ไปที่ PORTD
    PORTB = (ADCL >> 6);
  }
}