#define Y_LED 0   // Yellow LED connected to GPIO0
#define R_LED 15  // Red LED connected to GPIO15
#define G_LED 2   // Green LED connected to GPIO2
#define DIP12 12  // DIP switch bit 5 connected to GPIO12
#define DIP13 13  // DIP switch bit 6 connected to GPIO13
#define DIP5 5    // DIP switch bit 7 connected to GPIO5
#define DIP16 16  // DIP switch bit 8 connected to GPIO16
#define DIP4 4    // DIP switch bit 1 connected to GPIO4
#define DIP14 14  // DIP switch bit 2 connected to GPIO14

uint8_t mode = 0;

#define BLYNK_TEMPLATE_ID "TMPL6M7I73c9e"
#define BLYNK_TEMPLATE_NAME "checkpoint3"
#define BLYNK_AUTH_TOKEN "N4VUWZFC0SVN9QlJOTmB6jfBdtaSpcSi"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// char ssid[] = "CoEIoT";
// char pass[] = "iot.coe.psu.ac.th";

char ssid[] = "NaTtanon";
char pass[] = "nattanon";

BlynkTimer timer;

// Blynk write handlers for virtual pins
BLYNK_WRITE(V0) {
  if (mode == 9 || digitalRead(DIP5)) {
    digitalWrite(R_LED, param.asInt());
  }
}

BLYNK_WRITE(V1) {
  if (mode == 10 || digitalRead(DIP5)) {
    digitalWrite(G_LED, param.asInt());
  }
}

BLYNK_WRITE(V2) {
  if (mode == 11 || digitalRead(DIP5)) {
    digitalWrite(Y_LED, param.asInt());
  }
}

// Function to be called when connected to Blynk
BLYNK_CONNECTED() {
  Blynk.setProperty(V100, "offImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V100, "onImageUrl", "https://staticimage.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V100, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstartdevice-was-made");
}

// Functions to read DIP switch states and send to Blynk
void ReadSW1() {
  Blynk.virtualWrite(V3, digitalRead(DIP4));
}

void ReadSW2() {
  Blynk.virtualWrite(V4, digitalRead(DIP14));
}

void initial() {
  pinMode(Y_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(DIP12, INPUT);
  pinMode(DIP13, INPUT);
  pinMode(DIP5, INPUT);
  pinMode(DIP16, INPUT);
  pinMode(DIP4, INPUT);
  pinMode(DIP14, INPUT);
}

void setup() {
  initial();

  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(100L, ReadSW1);
  timer.setInterval(100L, ReadSW2);
}

unsigned long prevTime = 0;
bool ledState = false;

void loop() {
  Blynk.run();
  timer.run();

  mode = (digitalRead(DIP12) * 1) + (digitalRead(DIP13) * 2) + (digitalRead(DIP5) * 4) + (digitalRead(DIP16) * 8);

  switch (mode) {
    case 0:
      digitalWrite(G_LED, LOW);
      digitalWrite(Y_LED, LOW);
      digitalWrite(R_LED, LOW);
      break;

    case 5:
      if ((millis() - prevTime) > 1000) {
        prevTime = millis();
        ledState = !ledState;
      }
      digitalWrite(Y_LED, LOW);
      digitalWrite(G_LED, LOW);
      analogWrite(R_LED, ledState ? 205 : 0);
      break;

    case 6:
      if ((millis() - prevTime) > 1000) {
        prevTime = millis();
        ledState = !ledState;
      }
      digitalWrite(Y_LED, LOW);
      digitalWrite(R_LED, LOW);
      analogWrite(G_LED, ledState ? 512 : 0);
      break;

    case 7:
      if ((millis() - prevTime) > 1000) {
        prevTime = millis();
        ledState = !ledState;
      }
      digitalWrite(R_LED, LOW);
      digitalWrite(G_LED, LOW);
      analogWrite(Y_LED, ledState ? 716 : 0);
      break;

    case 8:
      if (millis() - prevTime > 1000) {
        prevTime = millis();
        ledState = !ledState;
      }
      digitalWrite(Y_LED, ledState);
      digitalWrite(G_LED, ledState);
      digitalWrite(R_LED, ledState);
      break;

    default:
      break;
  }
}
