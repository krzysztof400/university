#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "Wheels.h"

#define MS_PER_CM 50
#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


LiquidCrystal_I2C lcd(0x27, 16, 2); 

uint8_t arrowRight[8] = {
    0b01000,
    0b01100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b01100,
    0b01000
};

uint8_t arrowLeft[8] = {
    0b00010,
    0b00110,
    0b01100,
    0b11111,
    0b11111,
    0b01100,
    0b00110,
    0b00010
};

Wheels::Wheels() 
{ }

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}

void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
    
    lcd.init();
    lcd.backlight();
    
    // create our own chars in lcd memory
    lcd.createChar(0, arrowRight); 
    lcd.createChar(1, arrowLeft);  
    
    lcd.clear();
    lcd.print("Autko gotowe!");
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop()
{
    this->stopLeft();
    this->stopRight();
}

void Wheels::goForward(int cm)
{
    int currentSpeed = 200; 
    int totalTime = MS_PER_CM * cm; 
    int stepTime = 200;
    int totalSteps = totalTime / stepTime;
    
    if (totalSteps == 0) totalSteps = 1; 

    setSpeed(currentSpeed);
    forward(); 
    
    for (int i = 0; i <= totalSteps; i++) {
        int remaining_cm = cm - (cm * i / totalSteps);
        if (remaining_cm < 0) remaining_cm = 0;

        lcd.clear(); 
        
        // distacnce
        lcd.setCursor(0, 0);
        lcd.print("Zostalo: ");
        lcd.print(remaining_cm);
        lcd.print("cm");
        
        // speed
        lcd.setCursor(0, 1);
        lcd.print(currentSpeed); 
        
        lcd.setCursor(6, 1);
        int frame = i % 3; 
        if (frame == 0) { lcd.write(0); lcd.print("  "); }
        else if (frame == 1) { lcd.print(" "); lcd.write(0); lcd.print(" "); }
        else { lcd.print("  "); lcd.write(0); }

        lcd.setCursor(13, 1);
        lcd.print(currentSpeed);
        
        delay(stepTime);
    }
    
    stop(); 
    
    lcd.clear();
    lcd.print("Cel osiagniety!");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}

void Wheels::goBack(int cm)
{
    int currentSpeed = -200; 
    int totalTime = MS_PER_CM * cm; 
    int stepTime = 200; 
    int totalSteps = totalTime / stepTime;

    if (totalSteps == 0) totalSteps = 1;

    setSpeed(abs(currentSpeed)); 
    back(); 
    
    for (int i = 0; i <= totalSteps; i++) {
        int remaining_cm = cm - (cm * i / totalSteps);
        if (remaining_cm < 0) remaining_cm = 0;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("W tyl: ");
        lcd.print(remaining_cm);
        lcd.print("cm");
        
        // speed
        lcd.setCursor(0, 1);
        lcd.print(currentSpeed); 
        
        // arrowLeft
        lcd.setCursor(6, 1);
        int frame = i % 3; 
        if (frame == 0) { lcd.print("  "); lcd.write(1); }
        else if (frame == 1) { lcd.print(" "); lcd.write(1); lcd.print(" "); }
        else { lcd.write(1); lcd.print("  "); }

        lcd.setCursor(12, 1);
        lcd.print(currentSpeed);
        
        delay(stepTime);
    }
    
    stop();
    
    lcd.clear();
    lcd.print("Zatrzymany (Tyl)");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}