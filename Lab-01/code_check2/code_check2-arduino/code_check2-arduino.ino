const int segmentA = 14;
const int segmentB = 15;
const int segmentC = 16;
const int segmentD = 17;
const int segmentE = 18;
const int segmentF = 19;
const int segmentG = 8;

const int sw0 = 0;
const int sw4 = 4;
const int sw7 = 7;

const int led1 = 9;
const int led2 = 10;
const int led3 = 11;

void display_0()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, LOW);
}

void display_1()
{
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_2()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_3()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_4()
{
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_5()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_6()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_7()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_8()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_9()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_A()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_B()
{
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_C()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, LOW);
}

void display_D()
{
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_E()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_F()
{
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

int readSwitch(int startPin)
{
  int value = 0;
  for(int i = 0; i < 4; i++)
  {
    value |= (digitalRead(startPin + i) << i);
  }
  return value;
}

void display7Segment(int value) {
  switch(value) {
    case 0: display_0(); break;
    case 1: display_1(); break;
    case 2: display_2(); break;
    case 3: display_3(); break;
    case 4: display_4(); break;
    case 5: display_5(); break;
    case 6: display_6(); break;
    case 7: display_7(); break;
    case 8: display_8(); break;
    case 9: display_9(); break;
    case 10: display_A(); break;
    case 11: display_B(); break;
    case 12: display_C(); break;
    case 13: display_D(); break;
    case 14: display_E(); break;
    case 15: display_F(); break;
  }
}

void setup() {
  for (int i = sw0; i <= sw7; i++) {
    pinMode(i, INPUT);
  }
  
  for (int i = segmentA; i <= segmentF; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(segmentG, OUTPUT);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop()
{
  int dip1 = readSwitch(sw0);
  int dip2 = readSwitch(sw4);
  int result;
  
  if (dip1 == dip2) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    result = dip1; // or dip2, they are equal
  } else if (dip1 < dip2) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    result = dip2 - dip1;
  } else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    result = dip1 - dip2;
  }
  
  display7Segment(result);
  delay(200); // Add a delay for stability
}
