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

// Spring-controller parameters (Hooke's-law-like controller)
const float SPRING_KP = 1.2f;   // proportional gain (cm -> PWM)
const float SPRING_KD = 0.6f;   // damping gain (cm/s -> PWM)
const int REST_DISTANCE_CM = 100; // desired rest distance (spring equilibrium)
const int MAX_PWM = 220;
const int DEADBAND_PWM = 8;

static int lastDistance = -1;
static unsigned long lastSampleMs = 0;

// Continuous spring-like controller: compute command from distance error and
// estimated relative velocity of obstacle (from distance change). Positive
// pwm -> move forward (reduce distance), negative pwm -> move backward.
static void springControlStep() {
  unsigned long now = millis();
  int d = (int)measureDistanceCm(90);

  if (d <= 0) {
    // no valid reading: stop safely
    w.stop();
    lastDistance = -1;
    lastSampleMs = now;
    delay(100);
    return;
  }

  float dt = 0.0f;
  float v_rel = 0.0f; // cm/s, positive when obstacle is moving away

  if (lastDistance >= 0 && lastSampleMs > 0) {
    dt = (now - lastSampleMs) / 1000.0f;
    if (dt > 0.001f) {
      v_rel = (d - lastDistance) / dt;
    }
  }

  lastDistance = d;
  lastSampleMs = now;

  int error = d - REST_DISTANCE_CM; // positive if too far

  // Hooke-like command with damping: we treat Kp*error as spring force,
  // and Kd*v_rel as damping (chase if obstacle moves away, brake if approaches)
  float cmd = SPRING_KP * (float)error + SPRING_KD * v_rel;

  // Clamp to PWM range
  if (cmd > MAX_PWM) cmd = MAX_PWM;
  if (cmd < -MAX_PWM) cmd = -MAX_PWM;

  int pwm = (int)cmd;

  // Small deadband to avoid jitter
  if (pwm > DEADBAND_PWM) {
    w.setSpeed((uint8_t)min(pwm, MAX_PWM));
    w.forward();
  } else if (pwm < -DEADBAND_PWM) {
    w.setSpeed((uint8_t)min(-pwm, MAX_PWM));
    w.back();
  } else {
    // Around equilibrium: hold position
    w.stop();
  }

  // Update display with current distance and target
  w.displaySonar(90, d);

  // short control interval
  delay(120);
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
  w.setSpeed(120);

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  scanServo.attach(SERVO_PIN);
  scanServo.write((SCAN_MIN_ANGLE + SCAN_MAX_ANGLE) / 2);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() == 0) {
    springControlStep();
    return;
  }

  while (Serial.available()) {
    handleCommand((char)Serial.read());
  }
}