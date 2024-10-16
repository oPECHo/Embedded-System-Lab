#include <Arduino_FreeRTOS.h>  //เรียกใช้คลังโปรแกรมสำหรับเรียกฟังก์ชัน Arduino_FreeRTOS.h
#define R_LED 3                //กำหนด PIN3 สำหรับแสดงแอลอีดี RED
#define G_LED 4                //กำหนด PIN4 สำหรับแสดงแอลอีดี GREEN
#define Y_LED 5                //กำหนด PIN5 สำหรับแสดงแอลอีดี YELLOW
#define O_LED 6                //กำหนด PIN5 สำหรับแสดงแอลอีดี ORANGE
#define PUSH_BTN 2             //กำหนด PIN2 สำหรับรับค่าจากการกดปุ่มสวิตช์
#define POTEN 14               //กำหนด PIN14 สำหรับรับค่าจาก Potentiometer
uint8_t mode = 0;              //สร้างตัวแปร mode เพื่อกำหนดโหมดการแสดงผลของแอลอีดี
unsigned int delay_time = 500; //ประกาศตัวแปร delay_time เป็นหน่วงเวลา 500ms ในการติด-ดับแอลอีดี

void setup() {
//สร้าง Task 3 ฟังก์ชัน
  xTaskCreate(Read_Button, "READ BUTTON", 128, NULL, 1, NULL);
  xTaskCreate(Read_Poten, "READ POTENTIOMETER", 128, NULL, 1, NULL);
  xTaskCreate(Display_LED, "DISPLAY LED", 128, NULL, 1, NULL);
}
//สร้าง Task Display_LED (Red, Green, Yellow, Orange LED)
//--เพื่อแสดง OUTPUT ของแอลอีดี
void Display_LED(void *pvParameter){
  pinMode(R_LED, OUTPUT); //กำหนด PINMODE R_LED เป็น OUTPUT
  pinMode(G_LED, OUTPUT); //กำหนด PINMODE G_LED เป็น OUTPUT
  pinMode(Y_LED, OUTPUT); //กำหนด PINMODE Y_LED เป็น OUTPUT
  pinMode(O_LED, OUTPUT); //กำหนด PINMODE O_LED เป็น OUTPUT

  while(1){         //มีการวนลูป
    if(mode == 0){
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 1){
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 2){
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));

    }else if(mode == 3){
      digitalWrite(O_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 1);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(R_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(G_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(Y_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
      digitalWrite(O_LED, 0);
      vTaskDelay(pdMS_TO_TICKS(delay_time));
    }
  }
}

void Read_Poten(void *pvParameter) {
  pinMode(POTEN, INPUT);

  while (1) {
    int sensorValue = analogRead(POTEN);
    // Map potentiometer value to delay time range (50 ms to 1500 ms)
    delay_time = map(sensorValue, 0, 1023, 50, 1500);
    vTaskDelay(pdMS_TO_TICKS(100)); // Stabilize readings
  }
}

void Read_Button(void *pvParameter){ //สร้าง Task Read_Button สำหรับอ่านค่าการกดปุ่มสวิตช์
  pinMode(PUSH_BTN, INPUT); //กำหนด PINMODE PUSH_BTN เป็น INPUT
  while(1){
    if(!digitalRead(PUSH_BTN)){ //ตรวจสอบว่ามีการกดปุ่มบนสวิตช์หรือไม่ ?
      vTaskDelay(pdMS_TO_TICKS(10));  
      if(!digitalRead(PUSH_BTN)){
        while(!digitalRead(PUSH_BTN)){};
        mode < 3 ? mode++ : mode = 0; 
      }
    }
  }
}
void loop() {} 