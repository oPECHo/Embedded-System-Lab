#include <avr/sleep.h>
#define INT0_PIN 2  // Define pin 2 for interrupt INT0
#define INT1_PIN 3  // Define pin 3 for interrupt INT1

volatile uint8_t count;  // Variable to hold the count value

void SLEEP_DISABLE()    //ฟังก์ชันสำหรับปิดทางไม่ให้ตัวประมวลผลเข้าสู่ภาวะหลับ
{                       //เริ่มฟังก์ชัน SLEEP_DISABLE
  SMCR &= 0xFE;         //สั่งให้บิต SE(บิตล่างสุด) ในเรจิสเตอร์ SMCR มีค่าตรรกะต่ำ
}                       //สิ้นสุดฟังก์ชัน SLEEP_DISABLE

void SLEEP_INITIALIZE(uint8_t m)    //ฟังก์ชันสำหรับเปิดทางให้ CPU สามารถเข้าสู่สภาวะหลับได้
{                                   //--และรับพารามิเตอร์ระบุค่าหมายเลขแบบวิธีของภาวะหลับ
  SMCR = (m << 1) | 0x01;           //ตั้งค่าแบบวิธีภาวะหลับ และตั้งให้บิต SE มีค่าตรรกะสูง
}   

void setup() {
  pinMode(INT0_PIN, INPUT_PULLUP);  // Set INT0 pin as input with internal pull-up resistor
  pinMode(INT1_PIN, INPUT_PULLUP);  // Set INT1 pin as input with internal pull-up resistor

  // Set up INT0 and INT1 interrupts
  EICRA = (1 << ISC01) | (1 << ISC11);  // INT0 and INT1 triggered on falling edge
  EIMSK = (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1 interrupts

  DDRD = 0xF0;  // Set PORTD lower nibble as output
  DDRB = 0x0F;  // Set PORTB upper nibble as output

  count = 255;         // Initialize count variable
  display_LED(count);  // Display the initial count value
}

void loop() {
  SLEEP_INITIALIZE(0);  //ตั้งค่าการทำงานในภาวะหลับแบบเดินเครื่องเปล่า
  sleep_cpu();          //สั่งให้ตัวประมวลผลเข้าสู่ภาวะหลับ
  SLEEP_DISABLE();      //เมื่อตื่นจากภาวะหลับ ให้ปิดการทางการเข้าสู่ภาวะหลับ
}

// Function to display LED values
void display_LED(uint8_t val) {
  PORTD = (val << 4);  // Output the upper 4 bits to PORTD
  PORTB = (val >> 4);  // Output the lower 4 bits to PORTB
}

// Interrupt Service Routine for INT0
ISR(INT0_vect) {
  // Debounce delay (not ideal but works for simple cases)
  delay(200);

  if (!digitalRead(INT0_PIN)) {               // Check if button is still pressed
    count = (count < 255) ? (count + 1) : 0;  // Increment count or reset to 0
    display_LED(~count);                      // Update the LED display
  }
}

// Interrupt Service Routine for INT1
ISR(INT1_vect) {
  // Debounce delay (not ideal but works for simple cases)
  delay(200);

  if (!digitalRead(INT1_PIN)) {             // Check if button is still pressed
    count = (count > 0) ? (count - 1) : 0;  // Decrement count or keep at 0
    display_LED(~count);                    // Update the LED display
  }
}
