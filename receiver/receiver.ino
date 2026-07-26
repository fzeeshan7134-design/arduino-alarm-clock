#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "ALARM";

const int BTN_PINS[3] = {2, 3, 4};
const int LED_PINS[3] = {5, 6, 9};

int sequence[3];
int userProgress = 0;

int lastButtonState[3];
unsigned long lastPressMillis[3] = {0, 0, 0};
const unsigned long DEBOUNCE_MS = 200;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(BTN_PINS[i], INPUT);
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    lastButtonState[i] = digitalRead(BTN_PINS[i]);
  }

  randomSeed(analogRead(A5));

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  generateAndPlaySequence();
}

void loop() {
  checkButtons();
}

void generateAndPlaySequence() {
  for (int i = 0; i < 3; i++) {
    sequence[i] = random(0, 3);
  }

  userProgress = 0;

  Serial.print("New sequence: ");

  for (int i = 0; i < 3; i++) {
    Serial.print(sequence[i] + 1);
    Serial.print(" ");
  }

  Serial.println();

  playSequence();
}

void playSequence() {
  delay(500);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[sequence[i]], HIGH);
    delay(500);
    digitalWrite(LED_PINS[sequence[i]], LOW);
    delay(250);
  }
}

void checkButtons() {
  for (int i = 0; i < 3; i++) {
    int state = digitalRead(BTN_PINS[i]);

    if (state == LOW && lastButtonState[i] == HIGH &&
        millis() - lastPressMillis[i] > DEBOUNCE_MS) {

      lastPressMillis[i] = millis();
      handlePress(i);
    }

    lastButtonState[i] = state;
  }
}

void handlePress(int buttonIndex) {
  digitalWrite(LED_PINS[buttonIndex], HIGH);
  delay(100);
  digitalWrite(LED_PINS[buttonIndex], LOW);

  if (buttonIndex == sequence[userProgress]) {
    userProgress++;

    Serial.print("Correct! Progress: ");
    Serial.println(userProgress);

    if (userProgress >= 3) {
      Serial.println("Sequence complete!");

      sendShutoff();

      delay(1000);

      generateAndPlaySequence();
    }

  } else {
    Serial.println("Wrong! Failed sequence.");

    flashFail();

    generateAndPlaySequence();
  }
}

void flashFail() {
  for (int r = 0; r < 3; r++) {

    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PINS[i], HIGH);
    }

    delay(150);

    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PINS[i], LOW);
    }

    delay(150);
  }
}

void sendShutoff() {
  char msg[16] = "SHUTOFF";

  bool success = radio.write(&msg, sizeof(msg));

  Serial.print("Sent SHUTOFF: ");
  Serial.println(success ? "success" : "FAILED");
}
