#include "TimerOne.h"
#include "Wheels.h"
#include <Servo.h>



Wheels w;
volatile char cmd;

// Sonar + servo pins (adjust if your wiring differs)
const int SONAR_TRIG = 9;
const int SONAR_ECHO = 2;
const int SERVO_PIN = 6;

Servo scanServo;

// parameters
const int SCAN_MIN_ANGLE = 30;
const int SCAN_MAX_ANGLE = 150;
const int DIST_THRESHOLD_CM = 20; // stop distance

long measureDistanceCm(int angle) {
  scanServo.write(angle);
  delay(200); // allow servo to move

  // send pulse
  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);

  long duration = pulseIn(SONAR_ECHO, HIGH, 30000); // timeout 30ms
  if (duration == 0) return -1;
  long distanceCm = (duration / 2) / 29.1; // speed of sound
  return distanceCm;
}

int scanFront(int &outDistance) {
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

void runSpeedSweep() {
  const int speedStart = 60;
  const int speedEnd = 240;
  const int speedStep = 20;
  const unsigned long sampleMs = 2000;

  Serial.println("speed_param,cm_s,left_ticks,right_ticks,duration_ms");

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
    unsigned long leftTicks = w.getLeftTicks();
    unsigned long rightTicks = w.getRightTicks();
    float avgTicks = ((float)leftTicks + (float)rightTicks) * 0.5f;
    float distanceCm = avgTicks / w.getPulsesPerCm();
    float cmPerSec = (distanceCm * 1000.0f) / (float)durationMs;

    Serial.print(speedValue);
    Serial.print(",");
    Serial.print(cmPerSec, 4);
    Serial.print(",");
    Serial.print(leftTicks);
    Serial.print(",");
    Serial.print(rightTicks);
    Serial.print(",");
    Serial.println(durationMs);

    w.showSpeedMeasurement(speedValue, cmPerSec);
    delay(900);
  }

  w.stop();
  Serial.println("speed sweep done");
}

void setup() {
  w.attach(7,8,5,12,11,10);
  w.setPulsesPerCm(1.0f);

  
  // initialize sonar + servo pins
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);
  scanServo.attach(SERVO_PIN);
  scanServo.write((SCAN_MIN_ANGLE + SCAN_MAX_ANGLE) / 2);

  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Turn: L/R");
  Serial.println("Stop: S");
  Serial.println("Measure speed curve: V");

}

void loop() {
  // regular scanning loop - display sonar and react
  while (Serial.available() == 0) {
    int bestDist;
    int bestAngle = scanFront(bestDist);

    if (bestDist < 0) {
      Serial.println("No sonar data");
    } else {
      Serial.print("Angle: "); Serial.print(bestAngle);
      Serial.print(" deg, Dist: "); Serial.print(bestDist); Serial.println(" cm");
      w.displaySonar(bestAngle, bestDist);
    }

    if (bestDist > 0 && bestDist <= DIST_THRESHOLD_CM) {
      // obstacle too close
      w.stop();
      delay(200);

      // decide which side is clearer: scan left and right
      int leftAngle = SCAN_MAX_ANGLE; // left side
      int rightAngle = SCAN_MIN_ANGLE; // right side
      int leftDist = measureDistanceCm(leftAngle);
      int rightDist = measureDistanceCm(rightAngle);

      Serial.print("Left: "); Serial.print(leftDist); Serial.print(" cm, Right: "); Serial.print(rightDist); Serial.println(" cm");

      if (leftDist > rightDist) {
        // turn left and go around
        w.turnLeft(40);
        w.goForward(50);
      } else {
        // turn right and go around
        w.turnRight(40);
        w.goForward(50);
      }
    }

    delay(200);
  }

  while(Serial.available())
  {
    cmd = Serial.read();
    switch(cmd)
    {
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
      case 'f' : w.goForward(200); break;
      case 'b' : w.goBack(200); break;
      case 'v' : runSpeedSweep(); break;
      case 'V' : runSpeedSweep(); break;
    }
  }
}
