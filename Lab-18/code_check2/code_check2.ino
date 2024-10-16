#define Y_LED 0   //หลอดแอลอีดีสีเหลือง ต่อกับ GPIO0
#define R_LED 15  //หลอดแอลอีดีสีแดง ต่อกับ GPIO15
#define G_LED 2   //หลอดแอลอีดีสีเขียว ต่อกับ GPIO2
#define SW1 4     //ดิปสวิตช์ บิตที่ 1 ต่อกับ GPIO4
/*************************************************************
 This is a simple demo of sending and receiving some data.
 Be sure to check out other examples!
 *************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL6CFqXHboW"
#define BLYNK_TEMPLATE_NAME "checkpoint2"
#define BLYNK_AUTH_TOKEN "4QZ8VJiEAqIuAZRb6zviQZ-cxVxV1T1r"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
// char ssid[] = "CoEIoT";
// char pass[] = "iot.coe.psu.ac.th";

char ssid[] = "NaTtanon";
char pass[] = "nattanon";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if (value == 1)
    digitalWrite(R_LED, HIGH);
  else
    digitalWrite(R_LED, LOW);
}
BLYNK_WRITE(V1) {
  int value = param.asInt();
  analogWrite(G_LED, value);
}
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V100, "offImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V100, "onImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V100, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstartdevice-was-made");
}
// This function sends Arduino's uptime every second to Virtual Pin 2.

void ReadSW() {
  uint8_t d = digitalRead(SW1);
  Blynk.virtualWrite(V3, d);
}
void setup() {
  pinMode(Y_LED, OUTPUT);  //ตั้งให้ GPIO16 ซึ่งต่อกับแอลอีดีสีเหลืองทําหน้าที่เป็นเอาต์พุต
  pinMode(G_LED, OUTPUT);  //ตั้งให้ GPIO5 ซึ่งต่อกับแอลอีดีสีแดงทําหน้าที่เป็นเอาต์พุต
  pinMode(R_LED, OUTPUT);  //ตั้งให้ GPIO3 ซึ่งต่อกับแอลอีดีสีแดงทําหน้าที่เป็นเอาต์พุต
  pinMode(SW1, INPUT);     //ตั้งให้ GPIO4 ซึ่งต่อกับสวิตช์บิตที่ 1 ทำหน้าที่อินพุต
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Setup a function to be called every 100 ms
  timer.setInterval(100L, ReadSW);
}
void loop() {
  Blynk.run();
  timer.run();
  if (digitalRead(SW1)) {
    digitalWrite(Y_LED, HIGH);
  }
  digitalWrite(Y_LED, LOW);
}