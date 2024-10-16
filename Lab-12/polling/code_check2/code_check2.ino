int soundPin = 2;
int relayPin = 5;
unsigned long previous_time=0;

bool check_clap() {
    static unsigned long current_time;
    char sw;
    if (sw) {
        return false;
    } else {
        current_time = millis();
        if ((current_time - previous_time) > 50 && (current_time - previous_time) < 500) {
            previous_time = millis();
            return true;
        } else {
            previous_time = millis();
            return false;
        }
    }
}

int val;
int state;
int num_clap;

void setup() {
    EICRA |= (1 << CS00);
    EIMSK |= (1 << INT0);
    pinMode(relayPin, OUTPUT);
    pinMode(soundPin, INPUT);
    Serial.begin (9600);
    state = 0;
    num_clap = 0;
    Serial.println("num_clap = 0");
}

ISR(INT0_vect) {
    val = check_clap();

    if (val == true) {
        Serial.print("Clap deteected!!! #");
        Serial.print(num_clap);
        num_clap++;

        if (num_clap%2 == 1) {
            state = 0;
            Serial.println(" LED is turned off");
        } else {
            state = 1;
            Serial.println(" LED is turned on");
        }
    }

    if (state == 1)
        digitalWrite(relayPin, HIGH);
    else
        digitalWrite(relayPin, LOW);
}

void loop()
{

}