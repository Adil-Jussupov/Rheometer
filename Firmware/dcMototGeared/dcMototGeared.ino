// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
// author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

// #define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"
#define REMOTEXY_BLUETOOTH_NAME "DC_Mot_Swimmer"

#include "BLEDevice.h"
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 117 bytes
  { 255, 4, 0, 4, 0, 110, 0, 19, 0, 0, 0, 83, 119, 105, 109, 109, 105, 110, 82, 111,
    98, 111, 116, 0, 17, 2, 106, 200, 200, 140, 1, 1, 5, 0, 1, 17, 91, 32, 32, 92,
    13, 17, 17, 0, 13, 31, 43, 0, 1, 60, 90, 32, 32, 84, 62, 17, 17, 0, 13, 31,
    45, 0, 2, 32, 9, 45, 20, 19, 95, 83, 15, 1, 13, 26, 31, 31, 79, 78, 0, 79,
    70, 70, 0, 67, 12, 45, 85, 23, 51, 11, 75, 7, 78, 13, 26, 3, 2, 30, 145, 45,
    20, 79, 72, 85, 14, 1, 13, 26, 31, 31, 79, 78, 0, 79, 70, 70, 0 };

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


#define MOTPIN D7

bool debounceFlag = false;
long debounceTimer = 0;

long timeCounter = 100;
bool updFlag = 0;
int speedVar = 0;

int motorSpeed = 0;       // speed of the motor in 0-255
//int minMotorSpeed = 100;  // value when motor doesn't spin
int maxRPM = 1000;           // max RPM at motorSpeed = 255

CRemoteXY *remotexy;

void setup() {
  remotexy = new CRemoteXY(
    RemoteXY_CONF_PROGMEM,
    &RemoteXY,
    new CRemoteXYStream_BLEDevice(
      REMOTEXY_BLUETOOTH_NAME  // REMOTEXY_BLUETOOTH_NAME
      ));
  //  RemoteXY_Init();
  pinMode(MOTPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);  // LOW - LED is ON, HIGH - LED is OFF
  analogWrite(MOTPIN, 0);
}

void loop() {
  remotexy->handler();
  digitalWrite(LED_BUILTIN, (RemoteXY.switch_01 == 0) ? HIGH : LOW);
  if (RemoteXY.switch_01 == 0) analogWrite(MOTPIN, 0);

  // if (RemoteXY.switch_02 == 0) {
  //   // do something
  // } else {
  //   // or other thing
  // }

  if (!debounceFlag && RemoteXY.button_01) {
    debounceFlag = true;
    debounceTimer = millis();

    motorSpeed = motorSpeed + 1;
    if (motorSpeed < 50) motorSpeed = 50;
    if (motorSpeed > 100) motorSpeed = 100;
    RemoteXY.value_01 = float(motorSpeed)/10;
    analogWrite(MOTPIN, map(motorSpeed, 0, 100, 0, 255));
  }

  if (!debounceFlag && RemoteXY.button_02) {
    debounceFlag = true;
    debounceTimer = millis();

    motorSpeed = motorSpeed - 1;
    if (motorSpeed < 50) motorSpeed = 50;
    RemoteXY.value_01 = float(motorSpeed)/10;
    analogWrite(MOTPIN, map(motorSpeed, 0, 100, 0, 255));
  }

  if (debounceFlag && (millis() - debounceTimer) > 200) {
    debounceFlag = false;
  }
}
