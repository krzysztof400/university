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

// --- SERVO BIAS CONFIGURATION ---
// Adjust this value if your servo is physically rotated.
// For example, if 90 degrees logically actually points slightly right,
// try a negative number like -5 or -10 to center it.
const int SERVO_OFFSET = -5; 

const int SCAN_MIN_ANGLE = 30;
const int SCAN_MAX_ANGLE = 150;
const int DIST_THRESHOLD_CM = 30;

static void setServoAngle(int logicalAngle) {
    int physicalAngle = logicalAngle + SERVO_OFFSET;
    physicalAngle = constrain(physicalAngle, 0, 180);
    
    scanServo.write(physicalAngle);
}

static long measureDistanceCm(int angle) {
  setServoAngle(angle);
  delay(150);

  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);

  long duration = pulseIn(SONAR_ECHO, HIGH, 30000);
  long distance = -1;
  
  if (duration != 0) {
      distance = (duration / 2) / 29.1;
  }

  w.displaySonar(angle, (int)distance);

  return distance;
}

static int scanForBestDirection(int &outDistance) {
  int bestAngle = 90;
  long bestDist = -1;

  for (int a = SCAN_MIN_ANGLE; a <= SCAN_MAX_ANGLE; a += 20) {
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
    int quickScanAngles[] = {90, 75, 105}; 
    bool blocked = false;

    for (int i = 0; i < 3; i++) {
        int dist = (int)measureDistanceCm(quickScanAngles[i]);
        if (dist > 0 && dist <= DIST_THRESHOLD_CM) {
            blocked = true;
            break;
        }
    }

    // If the path is clear, continue driving
    if (!blocked) {
        w.forward();
        return;
    }

    // --- OBSTACLE DETECTED ---
    w.stop();
    delay(200);

    int bestDist = -1;
    int bestAngle = scanForBestDirection(bestDist);

    if (bestDist <= DIST_THRESHOLD_CM) {
        // Both sides are blocked, back up
        w.goBack(30);
        delay(500);
        w.stop();
    } else if (bestAngle > 90) {
        // More space on the left
        w.turnLeft(20);
        delay(500);
        w.stop();
    } else {
        w.turnRight(20);
        delay(500);
        w.stop();
    }
    
    setServoAngle(90);
    delay(150);
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
  }
}

void setup() {
  w.attach(7, 8, 5, 12, 13, 6);
  w.setPulsesPerCm(2.0f);
  w.setSpeed(90);

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  scanServo.attach(SERVO_PIN);
  setServoAngle(90);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() == 0) {
    avoidObstacleIfNeeded();
    return;
  }

  while (Serial.available()) {
    handleCommand((char)Serial.read());
  }
}