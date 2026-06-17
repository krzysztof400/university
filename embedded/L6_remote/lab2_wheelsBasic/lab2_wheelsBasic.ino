#define IR_RECEIVE_PIN 2
#include <IRremote.hpp>
#include <Arduino.h>
#include "Wheels.h"

Wheels w;

// Set this to the codes you discover from your remote.
// Use the serial monitor output from discovery mode to fill these values in.
const uint32_t PIN_CODE[4] = {
  0x00FF30CF,
  0x00FF18E7,
  0x00FF7A85,
  0x00FF10EF
};
const uint32_t ENTER_CODE = 0x00FF38C7;

// Remote command codes. Update them after you discover the values on your board.
const uint32_t CMD_FORWARD = 0x00FF18E7;
const uint32_t CMD_BACK = 0x00FF4AB5;
const uint32_t CMD_LEFT = 0x00FF10EF;
const uint32_t CMD_RIGHT = 0x00FF5AA5;
const uint32_t CMD_STOP = 0x00FF38C7;
const uint32_t CMD_SPEED1 = 0x00FF6897;
const uint32_t CMD_SPEED2 = 0x00FF9867;
const uint32_t CMD_SPEED3 = 0x00FFB04F;
const uint32_t CMD_SPEED4 = 0x00FF30CF;
const uint32_t CMD_SPEED5 = 0x00FF7A85;

const uint32_t COMMAND_DEBOUNCE_MS = 180;

bool unlocked = false;
uint8_t pinIndex = 0;
uint32_t lastAcceptedCode = 0;
uint32_t lastAcceptedTime = 0;

static void printCodeLine(const IRData &data) {
  Serial.print(F("IR protocol="));
  Serial.print((int)data.protocol);
  Serial.print(F(" address=0x"));
  Serial.print(data.address, HEX);
  Serial.print(F(" command=0x"));
  Serial.print(data.command, HEX);
  Serial.print(F(" raw=0x"));
  Serial.println((uint32_t)data.decodedRawData, HEX);
}

static bool isRepeatFrame(const IRData &data) {
  return (data.flags & IRDATA_FLAGS_IS_REPEAT) != 0;
}

static bool acceptCode(uint32_t code) {
  uint32_t now = millis();

  if (code == lastAcceptedCode && (now - lastAcceptedTime) < COMMAND_DEBOUNCE_MS) {
    return false;
  }

  lastAcceptedCode = code;
  lastAcceptedTime = now;
  return true;
}

static void unlockStep(uint32_t code) {
  if (code == PIN_CODE[pinIndex]) {
    pinIndex++;
    Serial.print(F("PIN step "));
    Serial.print(pinIndex);
    Serial.println(F("/4 OK"));

    if (pinIndex >= 4) {
      Serial.println(F("PIN entered, press ENTER"));
    }
    return;
  }

  if (code == ENTER_CODE && pinIndex == 4) {
    unlocked = true;
    w.stop();
    Serial.println(F("Car unlocked"));
    return;
  }

  pinIndex = 0;
  Serial.println(F("PIN reset"));
}

static void handleDriveCode(uint32_t code) {
  if (code == CMD_FORWARD) {
    w.forward();
  } else if (code == CMD_BACK) {
    w.back();
  } else if (code == CMD_LEFT) {
    w.forwardLeft();
  } else if (code == CMD_RIGHT) {
    w.forwardRight();
  } else if (code == CMD_STOP) {
    w.stop();
  } else if (code == CMD_SPEED1) {
    w.setSpeed(60);
  } else if (code == CMD_SPEED2) {
    w.setSpeed(90);
  } else if (code == CMD_SPEED3) {
    w.setSpeed(120);
  } else if (code == CMD_SPEED4) {
    w.setSpeed(160);
  } else if (code == CMD_SPEED5) {
    w.setSpeed(200);
  }
}

static void processIrFrame() {
  if (!IrReceiver.decode()) {
    return;
  }

  IRData data = IrReceiver.decodedIRData;
  printCodeLine(data);

  if (isRepeatFrame(data)) {
    IrReceiver.resume();
    return;
  }

  uint32_t code = (uint32_t)data.decodedRawData;
  w.displayRemoteCode(code, unlocked, pinIndex);

  if (!acceptCode(code)) {
    IrReceiver.resume();
    return;
  }

  if (!unlocked) {
    unlockStep(code);
  } else {
    handleDriveCode(code);
  }

  IrReceiver.resume();
}

static void waitForUnlock() {
  Serial.println(F("Enter PIN using remote, then press ENTER"));
  while (!unlocked) {
    processIrFrame();
  }
}

void setup() {
  w.attach(7, 8, 5, 12, 13, 6);
  w.setPulsesPerCm(2.0f);
  w.setSpeed(120);

  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println(F("IR remote discovery mode"));
  Serial.println(F("Press buttons and read codes from Serial Monitor"));
  waitForUnlock();
}

void loop() {
  processIrFrame();
}