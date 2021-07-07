#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;
int rs = 7;
int en = 8;
int d5 = 10;
int d6 = 11;
int d7 = 12;
int d4 = 9;

int trigPin = 3;
int echoPin = 2;
int pingTravelTime;
float distance;

int pos, buttonState;
int mode;
int buttonPin = 13;
int buzzState = LOW;

bool lastButtonState = HIGH;
bool newButtonState;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(4, OUTPUT);
  myservo.attach(5);

  Serial.begin(9600);
}

void normalMode() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pingTravelTime = pulseIn(echoPin, HIGH);
  delay(25);

  distance = ((pingTravelTime * 350.*100.) / 1000000) / 2;
  //  Serial.println(distance);
  if (distance < 20.) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello");
    lcd.setCursor(0, 1);
    lcd.print("Wellcome");
    while (pos <= 90) {
      myservo.write(pos);
      //    Serial.println(pos);
      delay(15);

      if (pos == 90) {
        myservo.write(90);
        delay(1);
        break;
      } pos += 1;
    }


  } else {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Goodbye");
    lcd.setCursor(0, 1);
    lcd.print("See you again");
    //  Serial.println(pos);
    while (pos > 0) {
      myservo.write(pos);
      //       Serial.println(pos);
      delay(15);
      pos -= 1;
    }
  }

}


void warningSound() {
  for (int i = 1000; i >= 0; i--) {
    digitalWrite(4, HIGH);
    delayMicroseconds(i);
    digitalWrite(4, LOW);
    delayMicroseconds(i);
  } for (int i = 0; i <= 1000; i++) {
    digitalWrite(4, HIGH);
    delayMicroseconds(i);
    digitalWrite(4, LOW);
    delayMicroseconds(i);
  }
}


void securityMode() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pingTravelTime = pulseIn(echoPin, HIGH);
  delay(25);

  distance = ((pingTravelTime * 350.*100.) / 1000000) / 2;

  if (distance < 20.) {
    if (distance < 15.) {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Warning");

      warningSound();
      lcd.setCursor(0, 1);
      lcd.print("Go Back or DIE");
      warningSound();

    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorry");
      lcd.setCursor(0, 1);
      lcd.print("We're closed");
    }
  } else {
    digitalWrite(4, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CLOSED");
    lcd.setCursor(0, 1);
    lcd.print("See you later");
  }
  delay(150);
}

int buttonListener() {
  int ButtonState = digitalRead(buttonPin);
  Serial.println(newButtonState);
  if (ButtonState == HIGH) {
    delay(180);
    newButtonState = !newButtonState;
  }
  if (newButtonState == HIGH) {
    return 2;
  } else {
    return 1;
  }
}

void loop() {

  int mode = buttonListener();
  Serial.println(mode);
  Serial.println(distance);
  switch (mode) {
    case 1:
      normalMode();
      break;

    case 2:
      while (pos > 0) {
        myservo.write(pos);
        //       Serial.println(pos);
        delay(15);
        pos -= 1;
      }
      securityMode();
      break;
  }
}
