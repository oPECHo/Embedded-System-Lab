#include <avr/interrupt.h>
#include <avr/sleep.h>

#define DATA_PIN A2
#define CLOCK_PIN A1
#define STROBE_PIN A0

const unsigned long DEBOUNCE_DELAY = 200;
const unsigned long BLINK_INTERVAL = 500;
const unsigned long INACTIVITY_TIMEOUT = 5000;  // 5 seconds timeout
volatile uint16_t millisRemaining = 12000;
volatile bool countStarted = false;
volatile bool countFinished = false;

unsigned long lastButtonDebounceTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastButtonPressTime = 0;  // Variable to track last button press time
bool displayOn = true;
uint8_t blinkCount = 0;

void SLEEP_DISABLE()  //ฟังก์ชันสำหรับปิดทางไม่ให้ตัวประมวลผลเข้าสู่ภาวะหลับ
{                     //เริ่มฟังก์ชัน SLEEP_DISABLE
  SMCR &= 0xFE;       //สั่งให้บิต SE(บิตล่างสุด) ในเรจิสเตอร์ SMCR มีค่าตรรกะต่ำ
}  //สิ้นสุดฟังก์ชัน SLEEP_DISABLE

void SLEEP_INITIALIZE(uint8_t m)  //ฟังก์ชันสำหรับเปิดทางให้ CPU สามารถเข้าสู่สภาวะหลับได้
{                                 //--และรับพารามิเตอร์ระบุค่าหมายเลขแบบวิธีของภาวะหลับ
  SMCR = (m << 1) | 0x01;         //ตั้งค่าแบบวิธีภาวะหลับ และตั้งให้บิต SE มีค่าตรรกะสูง
}

void sendCommand(uint8_t value) {
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);
  digitalWrite(STROBE_PIN, HIGH);
}

void resetDisplay() {
  sendCommand(0x40);
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0xc0);
  for (uint8_t i = 0; i < 16; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0x00);
  }
  digitalWrite(STROBE_PIN, HIGH);
}

uint8_t readButtons() {
  uint8_t buttons = 0;
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0x42);

  pinMode(DATA_PIN, INPUT);

  for (uint8_t i = 0; i < 4; i++) {
    buttons |= shiftIn(DATA_PIN, CLOCK_PIN, LSBFIRST) << i;
  }

  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(STROBE_PIN, HIGH);
  return buttons;
}

void displayDigit(uint8_t digit, uint8_t position) {
  sendCommand(0x44);
  digitalWrite(STROBE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0xc0 + (position << 1));
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digit);
  digitalWrite(STROBE_PIN, HIGH);
}

void displayTime(uint16_t millis) {
  const uint8_t digitPatterns[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
  };

  uint8_t seconds = millis / 1000;
  uint8_t hundredths = (millis % 1000) / 10;

  uint8_t s1 = digitPatterns[seconds / 10];
  uint8_t s2 = digitPatterns[seconds % 10];
  uint8_t h1 = digitPatterns[hundredths / 10];
  uint8_t h2 = digitPatterns[hundredths % 10];

  uint8_t displayValues[8] = { 0x00, 0x00, 0x00, s1, s2, 0x40, h1, h2 };
  if (!countFinished) {
    displayValues[0] = 0x5E;
    displayValues[1] = 0x54;
  }

  for (uint8_t i = 0; i < 8; i++) {
    displayDigit(displayValues[i], i);
  }
}

void setup() {
  pinMode(STROBE_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  sendCommand(0x8F);
  resetDisplay();

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 62;
  TCCR1B |= (1 << WGM12) | (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void loop() {
  uint8_t buttons = readButtons();
  unsigned long currentMillis = millis();

  if (currentMillis - lastButtonPressTime >= INACTIVITY_TIMEOUT && !countStarted) {
    resetDisplay();
    SLEEP_INITIALIZE(2);
    sleep_cpu();
    lastButtonPressTime = millis();
  }

  if ((buttons & 0x80) && (currentMillis - lastButtonDebounceTime > DEBOUNCE_DELAY)) {
    countStarted = true;
    lastButtonDebounceTime = currentMillis;
    lastButtonPressTime = currentMillis;
  }

  if (countFinished) {
    if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
      displayOn = !displayOn;
      lastBlinkTime = currentMillis;
      blinkCount++;

      if (blinkCount >= 5) {
        countFinished = false;
        resetDisplay();
        displayOn = false;
        resetDisplay();
        SLEEP_INITIALIZE(2);
      }
    }

    if (displayOn) {
      displayTime(millisRemaining);
    } else {
      resetDisplay();
    }
  } else {
    displayTime(millisRemaining);
  }
}

ISR(TIMER1_COMPA_vect) {
  if (countStarted && !countFinished) {
    if (millisRemaining == 0) {
      countFinished = true;
      lastBlinkTime = millis();
    } else {
      millisRemaining--;
    }
  }
}
