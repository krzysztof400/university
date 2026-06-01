#include "TimerOne.h"
#include "Wheels.h"



Wheels w;
volatile char cmd;

void setup() {
  w.attach(7,8,5,12,11,10);

  
  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Stop: S");

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
      case 's': w.stop(); break;
      case '1': w.setSpeed(1); break;
      case '2': w.setSpeed(2); break;
      case '3': w.setSpeed(3); break;
      case '4': w.setSpeed(4); break;
      case '5': w.setSpeed(5); break;
      case 'f' : w.goForward(200); break;
      case 'b' : w.goBack(200); break;
    }
  }
}
