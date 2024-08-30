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

// Include the AccelStepper Library
#include <AccelStepper.h>

// Motor Connections (constant current, step/direction bipolar motor driver)
const int dirPin = D5;
const int stepPin = D4;

// Speed control variables
int spinRate = 1;  // in RPM
String inputString = "";
bool stateUpdate = true;
int steps = 0;

// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
AccelStepper myStepper(AccelStepper::DRIVER, stepPin, dirPin);  // works for a4988 (Bipolar, constant current, step/direction driver)

void setup() {
  Serial.begin(9600);
  myStepper.setMaxSpeed(100000);  // this limits the value of setSpeed(). Raise it if you like.
  myStepper.setSpeed(2500);        // runSpeed() will run the motor at this speed - set it to whatever you like.
  myStepper.moveTo(10000);
  myStepper.moveTo(-10000);
}

void loop() {
  myStepper.runSpeed();  // This will run the motor forever.

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