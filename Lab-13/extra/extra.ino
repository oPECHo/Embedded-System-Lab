#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TM1637Display.h>
#include <SoftwareSerial.h>

#define DIO 2
#define CLK 4
#define RST_PIN 9
#define SS_PIN 10
#define BUZZER_PIN 8
#define LED_PIN 7

const int sw0 = 14;
const int sw1 = 15;
const int sw2 = 16;
const int sw3 = 17;
const int sw_restock = 5;
const byte rxPin = 0;
const byte txPin = 1;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
TM1637Display display(CLK, DIO);
SoftwareSerial mySerial(rxPin, txPin);

const int Contrast = 100;
int buttonPressed = -1;
bool isCardDetected = false;
bool cardEnabled = false;
bool timeout = false;
int count = 0;
int pulselen = 150;

const char* snacks[] = {"Snack A", "Snack B", "Snack C", "Snack D"};
int prices[] = {10, 20, 30, 40};
int product_amount[] = {4, 4, 4, 4};

// Timing variables
volatile unsigned long countdownMillis = 0; // Volatile because it's used in ISR

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);

    pinMode(sw_restock, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    analogWrite(6, Contrast);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("  Welcome to  ");
    lcd.setCursor(0, 1);
    lcd.print("Vending Machine");

    SPI.begin();
    rfid.PCD_Init();
    display.clear();
    display.setBrightness(7);

    // Timer0 setup
    noInterrupts();           // Disable interrupts
    TCCR0A = (1 << WGM01);   // Set Timer0 to CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00); // Set prescaler to 1024
    OCR0A = 249;             // Set the compare match register for 1 second
    TIMSK0 = (1 << OCIE0A);  // Enable Timer0 compare interrupt
    interrupts();             // Enable interrupts
}

void loop() {
    if (count > 0 && countdownMillis >= 1000) {
        updateCountdown();
        countdownMillis = 0; // Reset the milliseconds counter
    }

    int button = checkButtons();
    if (button != -1) {
        displaySnackAndPrice(button);
    }

    display.showNumberDecEx(00, 0b11100000, true, 2, 0);
    display.showNumberDecEx(count, 0, true, 2, 2);

    if (digitalRead(sw_restock) == LOW) {
        restockProduct();
    }

    if (timeout) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Time out!   ");
        lcd.setCursor(0, 1);
        lcd.print("Please try again");
        delay(1000); // Short delay to show timeout message
        setup(); // Reset the setup
    } else if (isCardDetected && cardEnabled && count == 0) {
        count = 15;  // Start the countdown from 15 seconds
        countdownMillis = 0; // Initialize countdownMillis
        timeout = false;
    }
}

int checkButtons() {
    for (int i = 0; i < 4; i++) {
        if (digitalRead(sw0 + i) == LOW) {
            if (product_amount[i] > 0) {
                enableCard();
                buttonPressed = sw0 + i;
                count = 15;  // Start the countdown timer
                countdownMillis = 0; // Initialize countdownMillis
                timeout = false;
                return buttonPressed;
            } else {
                count = 0;
                display.showNumberDecEx(0000, 0b11100000, true, 4, 0);
                displayOutOfStock(i);
                delay(1000); // Short delay to show out-of-stock message
                setup(); // Reset the setup
            }
        }
    }
    return -1; // No button pressed
}

void displaySnackAndPrice(int button) {
    int index = button - 14;
    if (product_amount[index] > 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(snacks[index]);
        lcd.print("  (");
        lcd.print(product_amount[index]);
        lcd.print(" left)");
        lcd.setCursor(0, 1);
        lcd.print("Price: ");
        lcd.print(prices[index]);
        lcd.print(" Bath");
    } else {
        displayOutOfStock(index);
    }
}

void displayOutOfStock(int index) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     ");
    lcd.print(snacks[index]);
    lcd.setCursor(0, 1);
    lcd.print("  OUT OF STOCK  ");
}

void readCard() {
    if (!cardEnabled) return;

    if (!rfid.PICC_IsNewCardPresent()) {
        isCardDetected = false;
        digitalWrite(LED_PIN, LOW);
        return;
    }

    if (!rfid.PICC_ReadCardSerial()) return;

    isCardDetected = true;
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(1000); // Delay is okay here as it's waiting for card processing
}

void enableCard() {
    cardEnabled = true;
}

void processTransaction() {
    digitalWrite(LED_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Processing...");
    digitalWrite(LED_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Enjoy   ");
    lcd.setCursor(0, 1);
    lcd.print(" your snack!!! ");
    updateProduct(buttonPressed);
    delay(1000); // Short delay to show the message
    setup(); // Reset the setup
}

void updateProduct(int buttonPressed) {
    int index = buttonPressed - 14;
    if (index >= 0 && index < 4 && product_amount[index] > 0) {
        product_amount[index]--;
        if (product_amount[index] == 0) {
            Serial.print(snacks[index]);
            Serial.println(" is out of stock ");
        } else {
            Serial.print(snacks[index]);
            Serial.print(" remaining ");
            Serial.println(product_amount[index]);
        }
    }
}

void restockProduct() {
    for (int i = 0; i < 4; i++) {
        product_amount[i] = 4;
    }
    Serial.println("Restock complete!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Restocking... ");
    delay(2000); // Short delay to show restocking message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Restock complete");
    delay(2000); // Short delay before resetting setup
    setup(); // Reset the setup
}
// Timer0 ISR for 1-second interval
ISR(TIMER1_COMPA_vect) {
  // Timer1 interrupt service routine
  seconds = 15;
  if (seconds != 0) {
    seconds++;
  }
  displayTime(seconds);
}

void displayTime(uint8_t seconds) {
  // Digit patterns for 7-segment display
  uint8_t digitPatterns[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
  };

  uint8_t s1 = digitPatterns[seconds / 10];
  uint8_t s2 = digitPatterns[seconds % 10];

  displayDigit(m1, 1);  // 1st digit of minutes
  displayDigit(m2, 2);  // 2nd digit of minutes
  displayDigit(s1, 3);  // 1st digit of seconds
  displayDigit(s2, 4);  // 2nd digit of seconds
}
