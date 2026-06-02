#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "TimerOne.h"
#include "Wheels.h"

#define MS_PER_CM 30
#define SENSOR_RIGHT A0
#define SENSOR_LEFT A1
#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

#define BEEPER 13

long int intPeroid = 500000;
bool reverseBeepEnabled = false;
volatile unsigned long rightWheelTicks = 0;
volatile unsigned long leftWheelTicks = 0;
float pulsesPerCm = 1.0f;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void doBeep() {
  digitalWrite(BEEPER, digitalRead(BEEPER) ^ 1);
}

void TimerUpdate() {
  Timer1.detachInterrupt();
  Timer1.attachInterrupt(doBeep, intPeroid);
}

void TimerStop() {
    Timer1.detachInterrupt();
}

static unsigned long readRightWheelTicks() {
    noInterrupts();
    unsigned long value = rightWheelTicks;
    interrupts();
    return value;
}

static unsigned long readLeftWheelTicks() {
    noInterrupts();
    unsigned long value = leftWheelTicks;
    interrupts();
    return value;
}

static void resetWheelTicks() {
    noInterrupts();
    rightWheelTicks = 0;
    leftWheelTicks = 0;
    interrupts();
}

static int distanceToTargetTicks(int cm) {
    int targetTicks = (int)(cm * pulsesPerCm + 0.5f);
    if (targetTicks <= 0) {
        targetTicks = 1;
    }

    return targetTicks;
}

static void setupWheelSensors() {
    pinMode(SENSOR_RIGHT, INPUT);
    pinMode(SENSOR_LEFT, INPUT);

    PCICR = 0x02;
    PCMSK1 = 0x03;
}

static void showProgress(const char *title, unsigned long leftTicks, unsigned long rightTicks, int targetTicks) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
    lcd.print(" ");
    lcd.print(leftTicks);
    lcd.print("/");
    lcd.print(targetTicks);

    lcd.setCursor(0, 1);
    lcd.print("P ");
    lcd.print(rightTicks);
    lcd.print("/");
    lcd.print(targetTicks);
}

ISR(PCINT1_vect) {
    if (PINC & (1 << PC0)) {
        rightWheelTicks++;
    }

    if (PINC & (1 << PC1)) {
        leftWheelTicks++;
    }
}


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
    intPeroid = max(50000L, (long)s * 1000L);

    if (reverseBeepEnabled) {
        TimerUpdate();
    }
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);

      
    pinMode(BEEPER, OUTPUT);
    Timer1.initialize();
    TimerStop();
    setupWheelSensors();
    resetWheelTicks();
    
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
    reverseBeepEnabled = false;
    TimerStop();
    this->forwardLeft();
    this->forwardRight();
}

void Wheels::back()
{
    reverseBeepEnabled = true;
    TimerUpdate();
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
    reverseBeepEnabled = false;
    TimerStop();
    this->stopLeft();
    this->stopRight();
}

void Wheels::goForward(int cm)
{
    int currentSpeed = 180;
    int targetTicks = distanceToTargetTicks(cm);
    unsigned long startTime = millis();
    unsigned long timeoutMs = max(1000L, (long)MS_PER_CM * cm * 2L);

    resetWheelTicks();
    setSpeed(currentSpeed);
    forward();

    while (true) {
        unsigned long leftTicks = readLeftWheelTicks();
        unsigned long rightTicks = readRightWheelTicks();

        if (leftTicks >= (unsigned long)targetTicks && rightTicks >= (unsigned long)targetTicks) {
            break;
        }

        if (millis() - startTime >= timeoutMs) {
            break;
        }

        showProgress("F", leftTicks, rightTicks, targetTicks);
        delay(50);
    }

    stop();

    lcd.clear();
    lcd.print("Cel osiagiety!");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}

void Wheels::goBack(int cm)
{
    int currentSpeed = 180;
    int targetTicks = distanceToTargetTicks(cm);
    unsigned long startTime = millis();
    unsigned long timeoutMs = max(1000L, (long)MS_PER_CM * cm * 2L);

    resetWheelTicks();
    setSpeed(currentSpeed);
    back();

    while (true) {
        unsigned long leftTicks = readLeftWheelTicks();
        unsigned long rightTicks = readRightWheelTicks();

        if (leftTicks >= (unsigned long)targetTicks && rightTicks >= (unsigned long)targetTicks) {
            break;
        }

        if (millis() - startTime >= timeoutMs) {
            break;
        }

        showProgress("B", leftTicks, rightTicks, targetTicks);
        delay(50);
    }

    stop();

    lcd.clear();
    lcd.print("Zatrzymany (Tyl)");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}

void Wheels::turnLeft(int cm)
{
    int currentSpeed = 170;
    int targetTicks = distanceToTargetTicks(cm);
    unsigned long startTime = millis();
    unsigned long timeoutMs = max(1000L, (long)MS_PER_CM * cm * 2L);

    resetWheelTicks();
    setSpeed(currentSpeed);
    reverseBeepEnabled = true;
    TimerUpdate();
    backLeft();
    forwardRight();

    while (true) {
        unsigned long leftTicks = readLeftWheelTicks();
        unsigned long rightTicks = readRightWheelTicks();

        if (leftTicks >= (unsigned long)targetTicks && rightTicks >= (unsigned long)targetTicks) {
            break;
        }

        if (millis() - startTime >= timeoutMs) {
            break;
        }

        showProgress("TL", leftTicks, rightTicks, targetTicks);
        delay(50);
    }

    stop();

    lcd.clear();
    lcd.print("Skret w lewo");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}

void Wheels::turnRight(int cm)
{
    int currentSpeed = 170;
    int targetTicks = distanceToTargetTicks(cm);
    unsigned long startTime = millis();
    unsigned long timeoutMs = max(1000L, (long)MS_PER_CM * cm * 2L);

    resetWheelTicks();
    setSpeed(currentSpeed);
    reverseBeepEnabled = true;
    TimerUpdate();
    forwardLeft();
    backRight();

    while (true) {
        unsigned long leftTicks = readLeftWheelTicks();
        unsigned long rightTicks = readRightWheelTicks();

        if (leftTicks >= (unsigned long)targetTicks && rightTicks >= (unsigned long)targetTicks) {
            break;
        }

        if (millis() - startTime >= timeoutMs) {
            break;
        }

        showProgress("TR", leftTicks, rightTicks, targetTicks);
        delay(50);
    }

    stop();

    lcd.clear();
    lcd.print("Skret w prawo");
    lcd.setCursor(0, 1);
    lcd.print("L: STOP  P: STOP");
}

void Wheels::resetEncoders()
{
    resetWheelTicks();
}

unsigned long Wheels::getLeftTicks()
{
    return readLeftWheelTicks();
}

unsigned long Wheels::getRightTicks()
{
    return readRightWheelTicks();
}

void Wheels::setPulsesPerCm(float value)
{
    if (value > 0.0f) {
        pulsesPerCm = value;
    }
}

float Wheels::getPulsesPerCm()
{
    return pulsesPerCm;
}

void Wheels::showSpeedMeasurement(int setSpeedValue, float cmPerSec)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("setSpeed: ");
    lcd.print(setSpeedValue);
    lcd.setCursor(0, 1);
    lcd.print("v=");
    lcd.print(cmPerSec, 2);
    lcd.print(" cm/s");
}
