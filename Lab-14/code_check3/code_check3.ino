#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>  //นำ KEYPAD มาใช้ในรูปแบบ I2C
#include <Keypad.h>
#include <Wire.h>
#include <TimeLib.h>  //TimeLibary
#include <DS1307RTC.h>

#define I2CADDR 0x20

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 7, 6, 5, 4 };
byte colPins[COLS] = { 3, 2, 1, 0 };

int Hour, Minute;
int Year, Month, Day;
unsigned long last_time;
unsigned int mode, digit, date;  //digit เอาไว้ check ว่ากดเลือก digit ไหน
bool blink_txt = false;
bool flag = false;  //ตรวจสอบว่ามีการแก้ไขหรือไม่
                    //flag = false ก็ต่อเมื่อกลับไปเป็นโหมดปกติ

LiquidCrystal_I2C lcd(0x27, 16, 2);  //Module IIC/I2C Interface
Keypad_I2C keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);

tmElements_t tm;  //for DS1307

void print2digit(int val) {
  if (val < 10 && val >= 0)
    lcd.print(0);
  lcd.print(val);
}

void setup() {
  Serial.begin(38400);
  Wire.begin();
  keypad.begin();
  lcd.init();
  lcd.backlight();  // เปิด backlight
  lcd.home();
  date = 0;
}

void loop() {
  if (RTC.read(tm)) {  //ตรวจสอบว่ามีการกดปุ่ม A หรือ C หรือ D หรือ #
    char key = keypad.getKey();
    
    if (key == 'B') {
      digit = !digit;
      mode = 1;
      flag = true;
      last_time = millis();  // Reset the blink timer
    }

    if (key == 'A') {
      date = (date < 2) ? date + 1 : 0;
      mode = 0;
      flag = true;
      last_time = millis();  // Reset the blink timer
    } else if (key == 'C') {
      if (!mode) {
        if (date == 0)
          Day = (Day < 31) ? Day + 1 : 1;
        else if (date == 1)
          Month = (Month < 12) ? Month + 1 : 1;
        else if (date == 2)
          Year = (Year < 2099) ? Year + 1 : 2099;
      } else {
        Serial.println("Time config");
        if (!digit)
          Hour = (Hour < 23) ? Hour + 1 : 0;
        else
          Minute = (Minute < 59) ? Minute + 1 : 0;
      }
    } else if (key == 'D') {
      if (!mode) {
        if (date == 0)
          Day = (Day > 1) ? Day - 1 : 31;
        else if (date == 1)
          Month = (Month > 1) ? Month - 1 : 12;
        else if (date == 2)
          Year = (Year > 0) ? Year - 1 : 2099;
      } else {
        if (!digit)
          Hour = (Hour > 0) ? Hour - 1 : 23;
        else
          Minute = (Minute > 0) ? Minute - 1 : 59;
      }
    } else if (key == '#') {
      tm.Day = Day;
      tm.Month = Month;
      tm.Year = CalendarYrToTm(Year);
      tm.Hour = Hour;
      tm.Minute = Minute;
      RTC.write(tm);
      flag = false;
    }

    lcd.setCursor(0, 0);
    lcd.print("D/M/Y=");
    if (flag && mode == 0) {
      if ((millis() - last_time) > 1000) {
        blink_txt = !blink_txt;
        last_time = millis();  // Reset the blink timer
      }
      if (date == 0) {
        if (blink_txt) {
          lcd.print("  ");
        } else {
          print2digit(Day);
        }
        lcd.print("/");
        print2digit(Month);
        lcd.print("/");
        lcd.print(Year);
      } else if (date == 1) {
        print2digit(Day);
        lcd.print("/");
        if (blink_txt) {
          lcd.print("  ");
        } else {
          print2digit(Month);
        }
        lcd.print("/");
        lcd.print(Year);
      } else if (date == 2) {
        print2digit(Day);
        lcd.print("/");
        print2digit(Month);
        lcd.print("/");
        if (blink_txt) {
          lcd.print("    ");
        } else {
          lcd.print(Year);
        }
      }
    } else {
      Day = tm.Day;
      Month = tm.Month;
      Year = tmYearToCalendar(tm.Year);
      print2digit(Day);
      lcd.print("/");
      print2digit(Month);
      lcd.print("/");
      print2digit(Year);
    }

    lcd.setCursor(0, 1);
    lcd.print("Time = ");
    if (flag && mode == 1) {
      if ((millis() - last_time) > 1000) {
        blink_txt = !blink_txt;
        last_time = millis();  // Reset the blink timer
      }
      if (!digit) {
        if (blink_txt) {
          lcd.print("  ");
        } else {
          print2digit(Hour);
        }
        lcd.print(":");
        print2digit(Minute);
      } else {
        print2digit(Hour);
        lcd.print(":");
        if (blink_txt) {
          lcd.print("  ");
        } else {
          print2digit(Minute);
        }
      }
    } else {
      Hour = tm.Hour;
      Minute = tm.Minute;
      print2digit(Hour);
      lcd.print(":");
      print2digit(Minute);
    }
  }
}
