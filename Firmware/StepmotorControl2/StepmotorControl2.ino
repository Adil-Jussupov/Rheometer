// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#define REMOTEXY_MODE__ESP32CORE_BLE



#define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"

#include <AccelStepper.h>

#include <BLEDevice.h>
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 93 bytes
  { 255, 4, 0, 2, 0, 86, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 5, 0,
    1, 29, 80, 48, 24, 1, 49, 31, 83, 112, 101, 101, 100, 32, 43, 0, 1, 29, 116, 49,
    24, 1, 175, 31, 83, 112, 101, 101, 100, 32, 45, 0, 1, 39, 159, 31, 24, 2, 120, 31,
    82, 101, 118, 101, 114, 115, 101, 0, 67, 20, 41, 69, 29, 85, 2, 26, 10, 42, 7, 26,
    26, 48, 4, 26, 31, 79, 78, 0, 31, 79, 70, 70, 0 };

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  uint8_t speedUp;        // =1 if button pressed, else =0
  uint8_t speedDown;      // =1 if button pressed, else =0
  uint8_t reverse;        // =1 if button pressed, else =0
  uint8_t pushSwitch_01;  // =1 if state is ON, else =0

  // output variables
  int16_t value_01;  // -32768 .. +32767

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)



const int dirPin = 27;
const int stepPin = 26;

const int enPin = 32;

const int ms1Pin = 33;
const int ms2Pin = 25;


bool debounceFlag = false;
long debounceTimer = 0;


AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);
//AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

long timeCounter = 1000;
bool updFlag = 0;
float speedVar = 1;
int stp = 400;  // steps per second for L-L 1/8 step

void setup() {
  RemoteXY_Init();

  pinMode(enPin, OUTPUT);
  pinMode(ms1Pin, OUTPUT);
  pinMode(ms2Pin, OUTPUT);

  digitalWrite(enPin, LOW);  // LOW - motor is ON, HIGH - motor is OFF

  // H-L - 1/2  step
  // L-H - 1/4  step
  // L-L - 1/8  step
  // H-H - 1/16 step
  digitalWrite(ms1Pin, LOW);
  digitalWrite(ms2Pin, LOW);

  stepper.setMaxSpeed(10000);
  stepper.setSpeed(speedVar*1);
}

void loop() {
  RemoteXY_Handler();
  if (!debounceFlag && RemoteXY.speedUp) {
    debounceFlag = true;
    debounceTimer = millis();
    speedVar++;
    stepper.setSpeed(speedVar*stp);
  }

  if (debounceFlag && (millis() - debounceTimer) > 500) {
    debounceFlag = false;
  }
  stepper.runSpeed();
  // if (millis() > timeCounter) {
  //   timeCounter = timeCounter + 1000;
  //   speedVar = speedVar - 500;
  //   stepper.setSpeed(speedVar);
  // }
}
