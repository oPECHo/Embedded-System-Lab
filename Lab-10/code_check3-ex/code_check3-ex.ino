
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // RS, E, D4, D5, D6, D7
void setup()
{
    lcd.begin(16, 2);
    
 lcd.setCursor(5,0);
 lcd.print("Custom");
 lcd.setCursor(3,1);
 lcd.print("Characters");
 delay(2000);
 lcd.clear();
}
void loop()
{
    image00();
    delay(250);
    image01();
    delay(250);
    image02();
    delay(250);
    image03();
    delay(700);
    image04();
    delay(250);
    image05();
    delay(250);
    image06();
    delay(700);
    image07();
    delay(1250);
}

void image00()
{
    lcd.clear();

    byte image22[8] = {B00110, B01101, B11011, B10011, B00111, B01111, B01111, B11111};
    byte image23[8] = {B01111, B11110, B11100, B11000, B11000, B10000, B10000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00111};
    byte image08[8] = {B00000, B01000, B10000, B10000, B10000, B11111, B11111, B11000};
    byte image09[8] = {B00000, B00000, B00000, B00000, B00000, B11000, B11000, B00100};

    lcd.createChar(0, image22);
    lcd.createChar(1, image23);
    lcd.createChar(2, image07);
    lcd.createChar(3, image08);
    lcd.createChar(4, image09);

    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(6, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 0);
    lcd.write(byte(4));
}

void image01()
{
    lcd.clear();

    byte image22[8] = {B00110, B00101, B00011, B00011, B00111, B01111, B01111, B11111};
    byte image23[8] = {B01111, B11110, B11100, B11000, B11000, B10000, B10000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B11001, B10111};
    byte image08[8] = {B00000, B01000, B10000, B10000, B10000, B11111, B11111, B11000};
    byte image09[8] = {B00000, B00000, B00000, B00000, B00000, B11000, B11000, B00100};
    byte image06[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00011};

    lcd.createChar(0, image22);
    lcd.createChar(1, image23);
    lcd.createChar(2, image07);
    lcd.createChar(3, image08);
    lcd.createChar(4, image09);
    lcd.createChar(5, image06);

    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(6, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 0);
    lcd.write(byte(4));
    lcd.setCursor(5, 0);
    lcd.write(byte(5));
}

void image02()
{
    lcd.clear();

    byte image22[8] = {B00000, B00001, B00011, B00011, B00111, B01111, B01111, B11111};
    byte image23[8] = {B01111, B11110, B11100, B11000, B11000, B10000, B10000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00001, B00111, B00100, B11001, B10111};
    byte image08[8] = {B00000, B01000, B10000, B10000, B10000, B11111, B11111, B11000};
    byte image09[8] = {B00000, B00000, B00000, B00000, B00000, B11000, B11000, B00100};

    lcd.createChar(0, image22);
    lcd.createChar(1, image23);
    lcd.createChar(2, image07);
    lcd.createChar(3, image08);
    lcd.createChar(4, image09);

    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(6, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 0);
    lcd.write(byte(4));
}

void image03()
{
    lcd.clear();

    byte image22[8] = {B00000, B00001, B00011, B00011, B00111, B01111, B01111, B11111};
    byte image23[8] = {B01111, B11110, B11100, B11000, B11000, B10000, B10000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00111};
    byte image08[8] = {B00000, B01000, B10000, B10000, B10000, B11111, B11111, B11010};
    byte image09[8] = {B00000, B00000, B00000, B00000, B00000, B11000, B11000, B00100};
    byte image24[8] = {B00010, B00111, B00111, B00111, B00111, B00111, B00010, B00000};

    lcd.createChar(0, image22);
    lcd.createChar(1, image23);
    lcd.createChar(2, image07);
    lcd.createChar(3, image08);
    lcd.createChar(4, image09);
    lcd.createChar(5, image24);

    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(6, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 0);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(5));
}

void image04()
{
    lcd.clear();

    byte image22[8] = {B00000, B00001, B00011, B00011, B00111, B01111, B01111, B11111};
    byte image23[8] = {B01111, B11110, B11100, B11000, B11000, B10001, B10000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00111};
    byte image08[8] = {B00000, B01000, B10000, B10000, B10000, B11111, B11111, B11010};
    byte image09[8] = {B00000, B00000, B00000, B00000, B00000, B11000, B11000, B00100};
    byte image24[8] = {B00010, B00100, B01011, B10101, B11010, B10101, B11010, B01110};
    byte image25[8] = {B00000, B00000, B00000, B10000, B10000, B00000, B00000, B00000};

    lcd.createChar(0, image22);
    lcd.createChar(1, image23);
    lcd.createChar(2, image07);
    lcd.createChar(3, image08);
    lcd.createChar(4, image09);
    lcd.createChar(5, image24);
    lcd.createChar(6, image25);

    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(6, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 0);
    lcd.write(byte(3));
    lcd.setCursor(8, 0);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(5));
    lcd.setCursor(8, 1);
    lcd.write(byte(6));
}

void image05()
{
    lcd.clear();

    byte image24[8] = {B01010, B10100, B01011, B10101, B11010, B10101, B11010, B01110};
    byte image25[8] = {B00000, B00000, B00000, B10000, B10000, B00000, B00000, B00000};
    byte image23[8] = {B01101, B01010, B01101, B00111, B00000, B00000, B00000, B00000};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00001, B00011};
    byte image08[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000};

    lcd.createChar(0, image24);
    lcd.createChar(1, image25);
    lcd.createChar(2, image23);
    lcd.createChar(3, image07);
    lcd.createChar(4, image08);

    lcd.setCursor(7, 1);
    lcd.write(byte(0));
    lcd.setCursor(8, 1);
    lcd.write(byte(1));
    lcd.setCursor(6, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 0);
    lcd.write(byte(3));
    lcd.setCursor(7, 0);
    lcd.write(byte(4));
}

void image06()
{
    lcd.clear();

    byte image08[8] = {B00000, B00100, B01010, B01010, B10001, B00011, B00110, B01100};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00001, B00010, B00010, B00001};
    byte image09[8] = {B00000, B00000, B10000, B11000, B00000, B00000, B11000, B00100};
    byte image24[8] = {B00100, B00100, B00011, B00000, B00000, B00000, B00000, B00000};
    byte image25[8] = {B10000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};

    lcd.createChar(0, image08);
    lcd.createChar(1, image07);
    lcd.createChar(2, image09);
    lcd.createChar(3, image24);
    lcd.createChar(4, image25);

    lcd.setCursor(7, 0);
    lcd.write(byte(0));
    lcd.setCursor(6, 0);
    lcd.write(byte(1));
    lcd.setCursor(8, 0);
    lcd.write(byte(2));
    lcd.setCursor(7, 1);
    lcd.write(byte(3));
    lcd.setCursor(8, 1);
    lcd.write(byte(4));
}

void image07()
{
    lcd.clear();

    byte image24[8] = {B10101, B01110, B01110, B00100, B10101, B01110, B00100, B11111};
    byte image08[8] = {B00000, B00100, B01010, B01010, B10001, B00011, B00110, B01100};
    byte image07[8] = {B00000, B00000, B00000, B00000, B00001, B00010, B00010, B00001};
    byte image09[8] = {B00000, B00000, B10000, B11000, B00000, B00000, B00000, B00000};

    lcd.createChar(0, image24);
    lcd.createChar(1, image08);
    lcd.createChar(2, image07);
    lcd.createChar(3, image09);

    lcd.setCursor(7, 1);
    lcd.write(byte(0));
    lcd.setCursor(7, 0);
    lcd.write(byte(1));
    lcd.setCursor(6, 0);
    lcd.write(byte(2));
    lcd.setCursor(8, 0);
    lcd.write(byte(3));
}
