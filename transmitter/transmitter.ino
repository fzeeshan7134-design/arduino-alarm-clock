#include <SPI.h>
#include <RF24.h>
#include <LiquidCrystal.h>

RF24 radio(7, 8); 
const byte address[6] = "ALARM";

LiquidCrystal lcd(9, 10, A0, A1, A2, A3);

const int BTN_NEXT = 2;
const int BTN_SET = A4;
const int BTN_UP = A5;
const int BUZZER = 6;

int setHours = 0;
int setMinutes = 5; 

bool armed = false;
unsigned long alarmTargetMillis = 0;
bool alarmActive = false;

enum Mode { NORMAL, SET_HOURS, SET_MINUTES };
Mode currentMode = NORMAL;

unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_MS = 200;

bool setButtonHeld = false;
unsigned long setPressStartMillis = 0;
const unsigned long LONG_PRESS_MS = 1500;
const unsigned long QUICK_TEST_MS = 60000UL; 

void setup() {
  Serial.begin(9600);

  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_SET, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Alarm Clock");
  delay(1000);
  lcd.clear();

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  handleButtons();
  checkAlarmTrigger();
  if (alarmActive) {
    ringBuzzer();
    checkForShutoffSignal();
  }
  updateDisplay();
}

void handleButtons() {

  bool setPressed = digitalRead(BTN_SET) == LOW;

  if (setPressed && !setButtonHeld) {
    setButtonHeld = true;
    setPressStartMillis = millis();
  } else if (!setPressed && setButtonHeld) {
    setButtonHeld = false;
    unsigned long heldFor = millis() - setPressStartMillis;

    if (heldFor >= LONG_PRESS_MS) {

      alarmTargetMillis = millis() + QUICK_TEST_MS;
      armed = true;
      alarmActive = false;
      currentMode = NORMAL;
      noTone(BUZZER);
    } else {
      currentMode = SET_HOURS;
      armed = false;
      alarmActive = false;
      noTone(BUZZER);
    }
    lastButtonPress = millis();
    return;
  }

  if (millis() - lastButtonPress < DEBOUNCE_MS) return;

  if (currentMode == SET_HOURS) {
    if (digitalRead(BTN_UP) == LOW) {
      lastButtonPress = millis();
      setHours = (setHours + 1) % 24;
    } else if (digitalRead(BTN_NEXT) == LOW) {
      lastButtonPress = millis();
      currentMode = SET_MINUTES;
    }
  } else if (currentMode == SET_MINUTES) {
    if (digitalRead(BTN_UP) == LOW) {
      lastButtonPress = millis();
      setMinutes = (setMinutes + 1) % 60;
    } else if (digitalRead(BTN_NEXT) == LOW) {
      lastButtonPress = millis();
      alarmTargetMillis = millis() + (unsigned long)setHours * 3600000UL
                                    + (unsigned long)setMinutes * 60000UL;
      armed = true;
      currentMode = NORMAL;
    }
  }
}

void checkAlarmTrigger() {
  if (armed && !alarmActive && millis() >= alarmTargetMillis) {
    alarmActive = true;
  }
}

void ringBuzzer() {
  unsigned long cyclePos = millis() % 1000;
  int freq;
  if (cyclePos < 500) {
    freq = map(cyclePos, 0, 500, 500, 1500);
  } else {
    freq = map(cyclePos, 500, 1000, 1500, 500);
  }
  tone(BUZZER, freq);
}

void checkForShutoffSignal() {
  if (radio.available()) {
    char msg[16] = "";
    radio.read(&msg, sizeof(msg));
    if (strcmp(msg, "SHUTOFF") == 0) {
      alarmActive = false;
      armed = false;
      noTone(BUZZER);
    }
  }
}

void updateDisplay() {
  lcd.setCursor(0, 0);

  if (currentMode == NORMAL) {
    if (alarmActive) {
      lcd.print("ALARM! GET UP! ");
      lcd.setCursor(0, 1);
      lcd.print("Press remote... ");
    } else if (armed) {
      unsigned long remaining = (alarmTargetMillis > millis()) ? (alarmTargetMillis - millis()) : 0;
      int remH = remaining / 3600000UL;
      int remM = (remaining % 3600000UL) / 60000UL;
      int remS = (remaining % 60000UL) / 1000UL;
      lcd.print("Ring in:        ");
      lcd.setCursor(0, 1);
      printTwoDigit(remH);
      lcd.print(":");
      printTwoDigit(remM);
      lcd.print(":");
      printTwoDigit(remS);
      lcd.print("        ");
    } else {
      lcd.print("Press SET to    ");
      lcd.setCursor(0, 1);
      lcd.print("set (hold=1min) ");
    }
  } else if (currentMode == SET_HOURS) {
    lcd.print("Set Hours:      ");
    lcd.setCursor(0, 1);
    lcd.print(setHours);
    lcd.print(" hour(s)  NEXT>>");
  } else if (currentMode == SET_MINUTES) {
    lcd.print("Set Minutes:    ");
    lcd.setCursor(0, 1);
    lcd.print(setMinutes);
    lcd.print(" min(s)  START>>");
  }
}

void printTwoDigit(int val) {
  if (val < 10) lcd.print("0");
  lcd.print(val);
}
