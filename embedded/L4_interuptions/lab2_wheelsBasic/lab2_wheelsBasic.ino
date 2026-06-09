#include "TimerOne.h"
#include "Wheels.h"



Wheels w;
volatile char cmd;

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

  
  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Turn: L/R");
  Serial.println("Stop: S");
  Serial.println("Measure speed curve: V");

}

void loop() {
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
      case 'a': w.left(); break;
      case 'd': w.right(); break;
      case 's': w.stop(); break;
      case '1': w.setSpeed(60); break;
      case '2': w.setSpeed(90); break;
      case '3': w.setSpeed(120); break;
      case '4': w.setSpeed(160); break;
      case '5': w.setSpeed(200); break;
      case 'f' : w.goForward(200); break;
      case 'b' : w.goBack(200); break;
      case 'v' : runSpeedSweep(); break;
    }
  }
}
