// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
// author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

// #define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"
#define REMOTEXY_BLUETOOTH_NAME "Swim_ESP32S3"

#include <AccelStepper.h>

#include "BLEDevice.h"
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 117 bytes
  { 255,4,0,4,0,110,0,19,0,0,0,83,119,105,109,109,105,110,82,111,
  98,111,116,0,17,2,106,200,200,140,1,1,5,0,1,17,91,32,32,92,
  13,17,17,0,13,31,43,0,1,60,90,32,32,84,62,17,17,0,13,31,
  45,0,2,32,9,45,20,19,95,83,15,1,13,26,31,31,79,78,0,79,
  70,70,0,67,12,45,85,23,51,11,75,7,78,13,26,3,2,30,145,45,
  20,79,72,85,14,1,13,26,31,31,79,78,0,79,70,70,0 };

// this structure defines all the variables and events of your control interface
// struct {

//   // input variables
//   uint8_t speedUp;        // =1 if button pressed, else =0
//   uint8_t speedDown;      // =1 if button pressed, else =0
//   uint8_t reverse;        // =1 if button pressed, else =0
//   uint8_t pushSwitch_01;  // =1 if state is ON, else =0

//   // output variables
//   int16_t value_01;  // -32768 .. +32767

//   // other variable
//   uint8_t connect_flag = 0;  // =1 if wire connected, else =0

// } RemoteXY;

struct {

  // input variables
  uint8_t button_01;      // =1 if button pressed, else =0
  uint8_t button_02;      // =1 if button pressed, else =0
  uint8_t switch_01 = 0;  // =1 if switch ON and =0 if OFF
  uint8_t switch_02 = 0;  // =1 if switch ON and =0 if OFF

  // output variables
  float value_01;  // -32768 .. +32767

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)


#define ENPIN D0

#define MS1PIN D1
#define MS2PIN D2

#define DIRPIN D4
#define STEPPIN D3

bool debounceFlag = false;
long debounceTimer = 0;


AccelStepper stepper(AccelStepper::DRIVER, STEPPIN, DIRPIN);
//AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

long timeCounter = 1000;
bool updFlag = 0;
int speedVar = 100;
int kSpeed = 100;  // speed multiplier
const int maxSpeed = 15000;
float stp = 0.5;           //
bool dirFlip = false;  // variable for rotation direction
int dirCoef = 1;


CRemoteXY *remotexy;

void setup() {
  remotexy = new CRemoteXY(
    RemoteXY_CONF_PROGMEM,
    &RemoteXY,
    new CRemoteXYStream_BLEDevice(
      "myRemoteXY"  // REMOTEXY_BLUETOOTH_NAME
      ));
  //  RemoteXY_Init();
  pinMode(ENPIN, OUTPUT);

  pinMode(MS1PIN, OUTPUT);
  pinMode(MS2PIN, OUTPUT);

  digitalWrite(ENPIN, HIGH);  // LOW - motor is ON, HIGH - motor is OFF

  // H-L - 1/2  step
  // L-H - 1/4  step
  // L-L - 1/8  step
  // H-H - 1/16 step
  digitalWrite(MS1PIN, HIGH);  // 1/16 step
  digitalWrite(MS2PIN, LOW);

  stepper.setMaxSpeed(maxSpeed + 200);
  //stepper.setAcceleration(10);
  stepper.setSpeed(100);
}

void loop() {
  remotexy->handler();
  digitalWrite(ENPIN, (RemoteXY.switch_01 == 0) ? HIGH : LOW);
  // RemoteXY_Handler();

  if (RemoteXY.switch_02 == 0) {
    dirCoef = 1;
    speedVar = dirCoef * stp * 160/3; // 3200/60
    RemoteXY.value_01 = dirCoef * stp;
    stepper.setSpeed(speedVar);
  }
  else {
    dirCoef = -1;
    speedVar = dirCoef * stp * 160/3;
    RemoteXY.value_01 = dirCoef * stp;
    stepper.setSpeed(speedVar);
  }

  if (!debounceFlag && RemoteXY.button_01) {
    debounceFlag = true;
    debounceTimer = millis();
    if ((speedVar) < (maxSpeed)) stp = stp + 0.05;
    speedVar = dirCoef * stp * 160/3;
    RemoteXY.value_01 = dirCoef * stp;
    stepper.setSpeed(speedVar);
  }

  if (!debounceFlag && RemoteXY.button_02) {
    debounceFlag = true;
    debounceTimer = millis();
    if ((speedVar) > 0.05) stp = stp - 0.05;
    speedVar = dirCoef * stp * 160/3;
    RemoteXY.value_01 = dirCoef * stp;
    stepper.setSpeed(speedVar);
  }

  if (debounceFlag && (millis() - debounceTimer) > 200) {
    debounceFlag = false;
  }
  stepper.runSpeed();
}
