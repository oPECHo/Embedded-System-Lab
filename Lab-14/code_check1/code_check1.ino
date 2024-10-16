#include <LiquidCrystal_I2C.h>
//#include <Keypad_I2C.h>
//#include <Keypad.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#define I2CADDR 0x20
//const byte ROWS = 4;
//const byte COLS = 4;
//char hexaKeys[ROWS][COLS] = {
// {'1','2','3','A'},
// {'4','5','6','B'},
// {'7','8','9','C'},
// {'*','0','#','D'}
//};
//
//byte rowPins[ROWS] = {7, 6, 5, 4};
//byte colPins[COLS] = {3, 2, 1, 0};
//unsigned long last_time;
//bool blink_txt;
LiquidCrystal_I2C lcd(0x27, 16, 2); //Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f
//Keypad_I2C keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR);
tmElements_t tm; //for DS1307

void setup()
{
 Serial.begin(38400);
 Wire.begin();
 //keypad.begin();
 lcd.init();
 lcd.backlight(); // เปิด backlight
 lcd.home(); 
 
}
void loop()
{
  if(RTC.read(tm)){
    lcd.setCursor(0,0);
    lcd.print("D/M/Y= ");
    lcd.print(tm.Day);
    lcd.print("/");
    lcd.print(tm.Month);
    lcd.print("/");
    lcd.print(tmYearToCalendar(tm.Year));
    lcd.setCursor(0, 1);
    lcd.print("Time = ");
    if(tm.Hour < 10 && tm.Hour > 0){
      lcd.print(0);
      lcd.print(tm.Hour);
    }else{
      lcd.print(tm.Hour);
    }
    lcd.print(":");
    if(tm.Minute < 10 && tm.Minute > 0){
      lcd.print(0);
      lcd.print(tm.Minute);
    }else{
      lcd.print(tm.Minute);
    }
  }
}