#include <Arduino.h>
#include <Servo.h>
#include "Wheels.h"

Wheels w;
volatile char cmd;

// Sonar + servo pins
const int SONAR_TRIG = 9;
const int SONAR_ECHO = 2;
const int SERVO_PIN  = 11;

Servo scanServo;

// Scan parameters
const int SCAN_MIN_ANGLE = 30;
const int SCAN_MAX_ANGLE = 150;
const int DIST_THRESHOLD_CM = 20;

static long measureDistanceCm(int angle) {
  scanServo.write(angle);
  delay(200); // allow servo to move

  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);

  long duration = pulseIn(SONAR_ECHO, HIGH, 30000);
  if (duration == 0) return -1;

  return (duration / 2) / 29.1;
}

static int scanFront(int &outDistance) {
  int bestAngle = (SCAN_MIN_ANGLE + SCAN_MAX_ANGLE) / 2;
  long bestDist = -1;

  for (int a = SCAN_MIN_ANGLE; a <= SCAN_MAX_ANGLE; a += 15) {
    long d = measureDistanceCm(a);
    if (d > bestDist) {
      bestDist = d;
      bestAngle = a;
    }
  }

  outDistance = (int)bestDist;
  return bestAngle;
}

static void avoidObstacleIfNeeded() {
    int frontDist = (int)measureDistanceCm(90);

    if (frontDist < 0 || frontDist > DIST_THRESHOLD_CM) {
        w.forward();
        return;
    }

    w.stop();
    delay(300);

    while (true) {

        int leftDist  = (int)measureDistanceCm(SCAN_MAX_ANGLE); // 150°
        int rightDist = (int)measureDistanceCm(SCAN_MIN_ANGLE); // 30°

        bool leftFree  = leftDist  > DIST_THRESHOLD_CM;
        bool rightFree = rightDist > DIST_THRESHOLD_CM;

        // free on left
        if (leftFree && leftDist >= rightDist) {
            w.turnLeft(40);
            delay(500);
            w.stop();
            break;
        }

        // free on right
        if (rightFree) {
            w.turnRight(40);
            delay(500);
            w.stop();
            break;
        }

        // both blocked
        w.goBack(30);
        delay(500);
        w.stop();
        delay(300);
    }
}

static void idleScanStep() {
  int bestDist = -1;
  int bestAngle = scanFront(bestDist);

  w.displaySonar(bestAngle, bestDist);

  avoidObstacleIfNeeded();

  delay(200);
}

static void runSpeedSweep() {
  const int speedStart = 60;
  const int speedEnd   = 240;
  const int speedStep   = 20;
  const unsigned long sampleMs = 2000;

  for (int speedValue = speedStart; speedValue <= speedEnd; speedValue += speedStep) {
    w.stop();
    w.resetEncoders();
    w.setSpeed((uint8_t)speedValue);
    delay(200);

    w.forward();

    unsigned long startTs = millis();
    while (millis() - startTs < sampleMs) {
      delay(20);
    }

    w.stop();

    unsigned long durationMs = millis() - startTs;
    unsigned long leftTicks  = w.getLeftTicks();
    unsigned long rightTicks = w.getRightTicks();

    float avgTicks = ((float)leftTicks + (float)rightTicks) * 0.5f;
    float distanceCm = avgTicks / w.getPulsesPerCm();
    float cmPerSec = (distanceCm * 1000.0f) / (float)durationMs;

    w.showSpeedMeasurement(speedValue, cmPerSec);
    delay(900);
  }

  w.stop();
}

static void handleCommand(char c) {
  switch (c) {
    case 'w': w.forward(); break;
    case 'x': w.back(); break;
    case 'a': w.forwardLeft(); break;
    case 'd': w.forwardRight(); break;
    case 'z': w.backLeft(); break;
    case 'c': w.backRight(); break;
    case 'l': w.turnLeft(20); break;
    case 'r': w.turnRight(20); break;
    case 's': w.stop(); break;

    case '1': w.setSpeed(60); break;
    case '2': w.setSpeed(90); break;
    case '3': w.setSpeed(120); break;
    case '4': w.setSpeed(160); break;
    case '5': w.setSpeed(200); break;

    case 'f': w.goForward(200); break;
    case 'b': w.goBack(200); break;

    case 'v':
    case 'V':
      runSpeedSweep();
      break;
  }
}

void setup() {
  w.attach(7, 8, 5, 12, 13, 6);
  w.setPulsesPerCm(2.0f);

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write((SCAN_MIN_ANGLE + SCAN_MAX_ANGLE) / 2);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() == 0) {
    idleScanStep();
    return;
  }

  while (Serial.available()) {
    handleCommand((char)Serial.read());
  }
}