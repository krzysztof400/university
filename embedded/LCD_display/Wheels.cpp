#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //[cite: 1]

#include "Wheels.h"

// Poprawione makro
#define MS_PER_CM 50
#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

// Inicjalizacja obiektu LCD (częsty adres 0x27 lub 0x3F)[cite: 1]
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Własny znak: Strzałka w prawo[cite: 1]
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

// Własny znak: Strzałka w lewo
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
    
    // Inicjalizacja LCD następuje podczas przypisywania pinów kół
    lcd.init(); //[cite: 1]
    lcd.backlight(); //[cite: 1]
    
    // Tworzenie własnych znaków strzałek w pamięci ekranu[cite: 1]
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
    int stepTime = 200; // Aktualizujemy ekran co 200 ms
    int totalSteps = totalTime / stepTime;
    
    // Zabezpieczenie przed podaniem np. 1 cm (czas całkowity < 200ms)
    if (totalSteps == 0) totalSteps = 1; 

    setSpeed(currentSpeed);
    forward(); 
    
    for (int i = 0; i <= totalSteps; i++) {
        int remaining_cm = cm - (cm * i / totalSteps);
        if (remaining_cm < 0) remaining_cm = 0;

        lcd.clear(); 
        
        // --- LINIA 1: Zostało odległości ---
        lcd.setCursor(0, 0);
        lcd.print("Zostalo: ");
        lcd.print(remaining_cm);
        lcd.print("cm");
        
        // --- LINIA 2: Deska rozdzielcza ---
        lcd.setCursor(0, 1);
        lcd.print(currentSpeed); 
        
        // Animacja na środku - znak "arrowRight"[cite: 1]
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
    
    lcd.clear(); //[cite: 1]
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

        lcd.clear(); //[cite: 1]
        lcd.setCursor(0, 0);
        lcd.print("W tyl: ");
        lcd.print(remaining_cm);
        lcd.print("cm");
        
        // Deska rozdzielcza
        lcd.setCursor(0, 1);
        lcd.print(currentSpeed); 
        
        // Animacja w tył - własny znak arrowLeft
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
    
    lcd.clear(); //[cite: 1]
    lcd.print("Zatrzymany (Tyl)");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}