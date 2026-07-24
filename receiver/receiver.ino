#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8); 
const byte address[6] = "ALARM"; 

const int BUTTON_PIN = 2;

int lastState;
unsigned long lastPressMillis = 0;
const unsigned long DEBOUNCE_MS = 250;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT); 

  lastState = digitalRead(BUTTON_PIN); 

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening(); 
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  if (state == LOW && lastState == HIGH && millis() - lastPressMillis > DEBOUNCE_MS) {
    lastPressMillis = millis();

    char msg[16] = "SHUTOFF";
    bool success = radio.write(&msg, sizeof(msg));

    Serial.print("Button pressed -> sent SHUTOFF: ");
    Serial.println(success ? "success" : "FAILED");
  }

  lastState = state;
}
