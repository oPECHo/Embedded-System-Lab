#include <Arduino_FreeRTOS.h>  //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน Arduino_FreeRTOS.h
#define R_LED 3                //กำหนด PIN3 สำหรับแสดงแอลอีดี RED
#define G_LED 4                //กำหนด PIN4 สำหรับแสดงแอลอีดี GREEN
#define Y_LED 5                //กำหนด PIN5 สำหรับแสดงแอลอีดี YELLOW
#define O_LED 6                //กำหนด PIN5 สำหรับแสดงแอลอีดี YELLOW
#define PUSH_SW 2              //กำหนด PIN2 สำหรับรับค่าจากการกดปุ่มสวิตช์
int display = 0;               //ประกาศตัวแปร display สำหรับแสดงผลแอลอีดี
//if display=1 then turn on only Red LED
//if display=2 then turn on only Green LED
//if display=3 then turn on only Yellow LED

void setup() {
  //สร้าง Task 4 ฟังก์ชัน
  Serial.begin(9600);
  xTaskCreate(Read_Switch, "Read push button switch", 128, NULL, 1, NULL);
  xTaskCreate(Display_R_LED, "Red LED Task", 128, NULL, 1, NULL);
  xTaskCreate(Display_G_LED, "Green LED Task", 128, NULL, 1, NULL);
  xTaskCreate(Display_Y_LED, "Yellow LED Task", 128, NULL, 1, NULL);
  xTaskCreate(Display_O_LED, "Orange LED Task", 128, NULL, 1, NULL);
  vTaskStartScheduler();  //Start the scheduler so the tasks start executing
}

void Display_R_LED(void *pvParameters)
//สร้าง Task Display_R_LED สำหรับ OUTPUT Red LED
{
  pinMode(R_LED, OUTPUT);  //กำหนด PINMODE R_LED เป็น OUTPUT
  while (1) {
    if (display == 1) {           //ตรวจสอบว่าค่าในตัวแปร display เท่ากับ 1 หรือไม่ ?
      digitalWrite(R_LED, HIGH);  //ถ้าค่าในตัวแปร display เท่ากับ 1 แอลอีดีสีแดงสว่าง
      Serial.print("R");
    } else {                     //ถ้าค่าในตัวแปร display ไม่เท่ากับ 1
      digitalWrite(R_LED, LOW);  //--แอลอีดีสีแดงดับ
    }
  }
}

void Display_G_LED(void *pvParameters)
//สร้าง Task Display_G_LED สำหรับ OUTPUT Green LED
{
  pinMode(G_LED, OUTPUT);  //กำหนด PINMODE G_LED เป็น OUTPUT
  while (1) {
    if (display == 2) {           //ตรวจสอบว่าค่าในตัวแปร display เท่ากับ 2 หรือไม่ ?
      digitalWrite(G_LED, HIGH);  //ถ้าค่าในตัวแปร display เท่ากับ 2 แอลอีดีสีเขียวสว่าง
      Serial.print("G");
    } else {                     //ถ้าค่าในตัวแปร display ไม่เก่ากับ 2
      digitalWrite(G_LED, LOW);  //--แอลอีดีสีเขียวดับ
    }
  }
}


void Display_Y_LED(void *pvParameters)
//สร้าง Task Display_Y_LED สำหรับ OUTPUT Yellow LED
{
  pinMode(Y_LED, OUTPUT);  //กำหนด PINMODE Y_LED เป็น OUTPUT
  while (1) {
    if (display == 3) {           //ตรวจสอบว่าค่าในตัวแปร display เท่ากับ 3 หรือไม่ ?
      digitalWrite(Y_LED, HIGH);  //ถ้าค่าในตัวแปร display เท่ากับ 3 แอลอีดีสีเหลืองสว่าง
      Serial.print("Y");
    } else {                     //ถ้าค่าในตัวแปร display ไม่เก่ากับ 3
      digitalWrite(Y_LED, LOW);  //--แอลอีดีสีเหลืองดับ
    }
  }
}

void Display_O_LED(void *pvParameters)
//สร้าง Task Display_Y_LED สำหรับ OUTPUT Yellow LED
{
  pinMode(O_LED, OUTPUT);  //กำหนด PINMODE Y_LED เป็น OUTPUT
  while (1) {
    if (display == 4) {           //ตรวจสอบว่าค่าในตัวแปร display เท่ากับ 3 หรือไม่ ?
      digitalWrite(O_LED, HIGH);  //ถ้าค่าในตัวแปร display เท่ากับ 3 แอลอีดีสีเหลืองสว่าง
      Serial.print("Y");
    } else {                     //ถ้าค่าในตัวแปร display ไม่เก่ากับ 3
      digitalWrite(O_LED, LOW);  //--แอลอีดีสีเหลืองดับ
    }
  }
}

void Read_Switch(void *pvParameters)
//สร้าง Task Display_Y_LED สำหรับอ่านค่าจากการกดปุ่มสวิตช์
{
  pinMode(PUSH_SW, INPUT);  //กำหนด PINMODE PUSH_SW เป็น INPUT
  while (1) {
    int sw_status = digitalRead(PUSH_SW);  //ประกาศตัวแปร sw_status เพื่อเก็บค่าการกดปุ่มสวิตช์
    if (sw_status == LOW) {
      vTaskDelay(pdMS_TO_TICKS(10));
      sw_status = digitalRead(PUSH_SW);
      if (sw_status == LOW) {  //ตรวจสอบว่ามีการกดปุ่มสวิตช์หรือไม่ ?
        while (sw_status == LOW) {
          sw_status = digitalRead(PUSH_SW);
        }
        display++;          //ตรวจสอบพบว่ามีการกดปุ่มสวิตช์ เพิ่มค่าในตัวแปร display ขึ้นหนึ่งค่า
        if (display > 4) {  //ตรวจสอบว่าค่าในตัวแปร display มากกว่า 3 หรือไม่ ?
          display = 0;      //ถ้าค่าในตัวแปร display มากกว่า 3
                            //--ให้กำหนดค่าในตัวแปร display เท่ากับ 0
        }
      }
    }
  }
}
void loop() {}