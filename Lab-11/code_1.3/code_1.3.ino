const int strobe = 7;
const int clock = 9;
const int data = 8;

void sendCommand(uint8_t value)
{
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

void setup()
{
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8f);  // activate 7segments and set maximum brightness
  reset();
  display_Hello();
}

void display_Hello()
{
  sendCommand(0x44);  // set single address

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // leftmost 7-segment digit 
  shiftOut(data, clock, LSBFIRST, 0x76); // character to be displayed ='H'
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc2); // 2rd 7-segment
  shiftOut(data, clock, LSBFIRST, 0x79); // character to be displayed ='E'
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc4); // 3rd 7-segment
  shiftOut(data, clock, LSBFIRST, 0x38); // character to be displayed ='L'
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc6); // 4th 7-segment
  shiftOut(data, clock, LSBFIRST, 0x38); // character to be displayed ='L'
  digitalWrite(strobe, HIGH);

  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc8); // 5th 7-segment
  shiftOut(data, clock, LSBFIRST, 0x3f); // character to be displayed ='O'
  digitalWrite(strobe, HIGH);
  
}

void loop()
{

}