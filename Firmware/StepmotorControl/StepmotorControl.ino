/*
   Uno sketch to drive a stepper motor using the AccelStepper library.
   Works with a ULN-2003 unipolar stepper driver, or a bipolar, constant voltage motor driver
   such as the L298 or TB6612, or a step/direction constant current driver like the a4988.
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations using runSpeed().
// Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
12/26/21  Modified to use various drivers vice default.   --jkl
*/
// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_BLE

#define ENAB D2

#include <AccelStepper.h>

#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"


#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 70 bytes
  { 255, 4, 0, 0, 0, 63, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 5, 0,
    1, 12, 45, 24, 24, 0, 1, 31, 0, 1, 14, 85, 24, 24, 0, 94, 31, 0, 1, 16,
    126, 24, 24, 0, 120, 31, 0, 1, 13, 7, 24, 24, 0, 2, 31, 0, 129, 52, 12, 40,
    11, 64, 1, 79, 110, 47, 79, 102, 102, 0 };


struct {

  // input variables
  uint8_t button_01;  // =1 if button pressed, else =0
  uint8_t button_02;  // =1 if button pressed, else =0
  uint8_t button_03;  // =1 if button pressed, else =0
  uint8_t button_04;  // =1 if button pressed, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

// Motor Connections (constant current, step/direction bipolar motor driver)
const int dirPin = D6;
const int stepPin = D5;

// Speed control variables
int spinRate = 1;  // in RPM
String inputString = "";
bool stateUpdate = true;
int steps = 500;
bool flag = HIGH;

bool onOff = LOW;
bool onOffState = LOW;
long currentOnOff = 0;

bool oscilation = LOW;
bool oscilationState = LOW;
long currentOscilation = 0;

// FULL = all pins a LOW = Speed in steps per second
// Polulu:  1/16 = all pins a HIGH = increase speed to 16.
// DRV9925: 1/32 = all pins a HIGH = increase speed to 32.

// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
AccelStepper myStepper(AccelStepper::DRIVER, stepPin, dirPin);  // works for a4988 (Bipolar, constant current, step/direction driver)

void setup() {
  RemoteXY_Init();

  Serial.begin(9600);
  myStepper.setMaxSpeed(100000);  // this limits the value of setSpeed(). Raise it if you like.
  myStepper.setSpeed(1000);       // runSpeed() will run the motor at this speed - set it to whatever you like.
  // myStepper.moveTo(10000);
  // myStepper.moveTo(-10000);
  //myStepper.runSpeed();  // This will run the motor forever.
  pinMode(ENAB, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(ENAB, HIGH); // Motor is Off by default 

  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
}

void loop() {

  RemoteXY_Handler();

  // if (RemoteXY.button_04 != 0) {
  //   onOff = !onOff;
  //   digitalWrite(D2, onOff);
  //   /*  button pressed */
  // }
  if (oscilationState) {
    if (millis() - currentOscilation >= 1000) oscilationState = LOW;
  }

  if (oscilationState == LOW && RemoteXY.button_03 != 0) {
    oscilationState = HIGH;
    currentOscilation = millis();
    oscilation = !oscilation;
    
    // Do something when Button 3 pressed
    spinRate = spinRate * (-1);
    stateUpdate = true;
    myStepper.setSpeed(spinRate);

    Serial.print("Oscilation is ");
    if (oscilation) Serial.println("on");
    else Serial.println("off");
  
  }

  if (onOffState) {
    if (millis() - currentOnOff >= 1000) onOffState = LOW;
  }

  if (onOffState == LOW && RemoteXY.button_04 != 0) {
    onOffState = HIGH;
    currentOnOff = millis();
    onOff = !onOff;
    
    // Do something when Button 3 pressed
    digitalWrite(ENAB, onOff);

    Serial.print("Motor is ");
    if (!onOff) Serial.println("on");
    else Serial.println("off");
    /*  button pressed */
  }

  myStepper.runSpeed();

  if (stateUpdate) {

    Serial.print("SpinRate: ");
    Serial.print(spinRate);
    Serial.print(" Units |  Steps: ");
    Serial.print(steps);
    Serial.print("\n--------------------------------------------\n");
    stateUpdate = false;
  }
}

void serialEventRun() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      if (inputString == "0") myStepper.stop();
      if (inputString.startsWith("d")) {
        steps = inputString.toInt();
        myStepper.move(steps);
      } else {
        spinRate = inputString.toInt();
        myStepper.setSpeed(spinRate);
      }
      stateUpdate = true;
      inputString = "";
    }
  }
}