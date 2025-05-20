#include "BluetoothSerial.h"

BluetoothSerial ESP_BT;

#define IN1 18  // Motor 1 (Voice-controlled) IN1
#define IN2 19  // Motor 1 (Voice-controlled) IN2
#define ENA 4   // PWM speed control for Motor 1

#define IN3 23  // Motor 2 (Cover-rolling) IN3
#define IN4 22  // Motor 2 (Cover-rolling) IN4
#define ENB 5   // PWM speed control for Motor 2

#define TRIG1 12
#define ECHO1 13
#define TRIG2 14
#define ECHO2 15
#define TRIG3 32
#define ECHO3 33

const float BOARD_HEIGHT = 35.0;   // cm
const float PVC_CIRCUMFERENCE = 6.0;  // cm
bool motor1Running = false;
bool coverActive = true; // Flag for cover movement activation
float previousDistance = BOARD_HEIGHT;

void setup() {
  Serial.begin(9600);
  ESP_BT.begin("ESP32_GearMotorControl");
  Serial.println("Bluetooth Device Ready to Pair");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
}

void controlMotor1(int direction, int speed) {
  analogWrite(ENA, speed);
  if (direction == 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (direction == -1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

void controlMotor2(int direction, int speed, int duration) {
  analogWrite(ENB, speed);
  if (direction == 1) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (direction == -1) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
    return;
  }
  delay(duration);
  controlMotor2(0, 0, 0);
}

float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  return duration == 0 ? BOARD_HEIGHT : duration / 58.0;
}

void loop() {
  if (ESP_BT.available()) {
    String command = ESP_BT.readStringUntil('\n');
    command.trim();
    Serial.print("Command received: ");
    Serial.println(command);

    if (command.equalsIgnoreCase("backward")) {
      controlMotor1(1, 100);
      motor1Running = true;
      Serial.println("Motor 1 Running Backward");
    } else if (command.equalsIgnoreCase("upward")) {
      controlMotor1(-1, 100);
      motor1Running = true;
      Serial.println("Motor 1 Running Upward");
    } else if (command.equalsIgnoreCase("stop")) {
      controlMotor1(0, 0);
      motor1Running = false;
      Serial.println("Motor 1 Stopped");
    } else if (command.equalsIgnoreCase("activate cover")) {
      coverActive = true;
      Serial.println("Cover activation enabled.");
    } else if (command.equalsIgnoreCase("deactivate cover")) {
      coverActive = false;
      Serial.println("Cover activation disabled.");
    }
  }

  if (coverActive) {
    float d1 = getDistance(TRIG1, ECHO1);
    float d2 = getDistance(TRIG2, ECHO2);
    float d3 = getDistance(TRIG3, ECHO3);
    float minDistance = min(d1, min(d2, d3));

    if (minDistance <= 25) {
      float rollupLength = BOARD_HEIGHT - minDistance;
      int rollupTime = (rollupLength / PVC_CIRCUMFERENCE) * 1000;
      Serial.print("Rolling Up Cover for: ");
      Serial.print(rollupTime);
      Serial.println(" ms");

      controlMotor2(1, 200, rollupTime);
      previousDistance = minDistance;
    } else if (minDistance > previousDistance) {
      float rolldownLength = minDistance - previousDistance;
      int rolldownTime = (rolldownLength / PVC_CIRCUMFERENCE) * 1000;
      Serial.print("Rolling Down Cover for: ");
      Serial.print(rolldownTime);
      Serial.println(" ms");

      controlMotor2(-1, 200, rolldownTime);
      previousDistance = minDistance;
    }
  }

  delay(100);
}
