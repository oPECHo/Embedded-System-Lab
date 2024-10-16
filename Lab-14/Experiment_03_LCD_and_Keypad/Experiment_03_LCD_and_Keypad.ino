#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>



#define I2CADDR 0x20


const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4};
byte colPins[COLS] = {3, 2, 1, 0};
unsigned long last_time;
bool blink_txt;

LiquidCrystal_I2C lcd(0x27, 16, 2);   //Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f
Keypad_I2C keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR); 


void setup()
{
  blink_txt = false;
  last_time =0;
  Wire.begin();
  keypad.begin();
  lcd.init();
  lcd.backlight();       // เปิด backlight 

  lcd.home();
  lcd.print("CoE PSU");
  lcd.setCursor(0, 1);
  lcd.print("Press Keypad...");  
}

void loop() 
{
  int i;
  char key = keypad.getKey();
  if (key != NO_KEY){    
    lcd.setCursor(0,1);
    for (i=0;i<16;i++)
      lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print(key);
    lcd.print(" is pressed");
  }

  //---ตรวจสอบว่าเวลาผ่านไป 1 วินาทีหรือยัง
  if( millis() - last_time > 1000)
  {
    last_time = millis();
    if(blink_txt)
    {
      blink_txt = false;
      lcd.setCursor(0,0);
      for(i=0;i<7;i++)
        lcd.print(" ");
    }
    else
    {
      blink_txt = true;
      lcd.setCursor(0,0);
      lcd.print("CoE PSU");
    }    
  }
}
