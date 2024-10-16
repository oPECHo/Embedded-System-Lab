const int segmentA = 14;
const int segmentB = 15;
const int segmentC = 16;
const int segmentD = 17;
const int segmentE = 18;
const int segmentF = 19;
const int segmentG = 8;

const int dg4 = 9;
  
const int COL = 4;
const int ROW = 4;

const int cols[4] = {3,2,1,0};
const int rows[4] = {7,6,5,4};
const int keyMap[4][4] =  {{1, 2, 3, 10}, {4, 5, 6, 11}, {7, 8, 9, 12}, {14, 0, 15, 13}};

void display_0(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, LOW);
}

void display_1(){
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_2(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_3(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_4(){
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_5(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_6(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_7(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, LOW);
}

void display_8(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_9(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, LOW); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_10(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_11(){
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_12(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, LOW);
}

void display_13(){
  digitalWrite(segmentA, LOW); digitalWrite(segmentB, HIGH);
  digitalWrite(segmentC, HIGH); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, LOW);
  digitalWrite(segmentG, HIGH);
}

void display_14(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, HIGH);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display_15(){
  digitalWrite(segmentA, HIGH); digitalWrite(segmentB, LOW);
  digitalWrite(segmentC, LOW); digitalWrite(segmentD, LOW);
  digitalWrite(segmentE, HIGH); digitalWrite(segmentF, HIGH);
  digitalWrite(segmentG, HIGH);
}

void display7Segment(int value) {
  switch(value){
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
    case 10: display_10(); break;
    case 11: display_11(); break;
    case 12: display_12(); break;
    case 13: display_13(); break;
    case 14: display_14(); break;
    case 15: display_15(); break;
  }
}

void setup() {
  pinMode(dg4, OUTPUT);
  for(int i = 0; i < ROW; i++) {
    pinMode(rows[i], INPUT_PULLUP);
    pinMode(cols[i], OUTPUT);
  }
  for (int i = segmentA; i <= segmentF; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(segmentG, OUTPUT);
  digitalWrite(dg4, HIGH);
}

void loop() {
  for(int i = 0; i < COL; i++) {
    digitalWrite(cols[i], LOW);
    for(int j = 0; j < ROW; j++) {
      if(digitalRead(rows[j]) == LOW) { 
        display7Segment(keyMap[j][i]); 
      }
    }
    digitalWrite(cols[i], HIGH); 
  }
} 