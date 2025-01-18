
#define REMOTEXY_MODE__ESP32CORE_BLE

#define ENAB 32

#include <AccelStepper.h>

#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"


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


// Motor Connections (constant current, step/direction bipolar motor driver)
const int dirPin = 27;
const int stepPin = 26;

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

bool speedUp = LOW;
bool speedUpState = LOW;
long currentSpeedUp = 0;

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
 // pinMode(32, OUTPUT);
 // pinMode(34, OUTPUT);

 // pinMode(32, OUTPUT);
 // pinMode(2, OUTPUT);
 // digitalWrite(25, HIGH);
 // digitalWrite(2, LOW);



  digitalWrite(ENAB, HIGH);  // Motor is Off by default

 // digitalWrite(32, HIGH);
 // digitalWrite(34, LOW);
}

void loop() {

  RemoteXY_Handler();

  if (speedUpState) {
    if (millis() - currentSpeedUp >= 1000) speedUpState = LOW;
  }

  if (speedUpState == LOW && RemoteXY.speedUp != 0) {
    speedUpState = HIGH;
    currentSpeedUp = millis();
    speedUp = !speedUp;

    // Do something when Button 3 pressed
    spinRate += 100;
    stateUpdate = true;
    myStepper.setSpeed(spinRate);
    RemoteXY.value_01 = spinRate;
    Serial.println("SpeedUp");
  }

  if (oscilationState == LOW && RemoteXY.reverse != 0) {
    oscilationState = HIGH;
    currentOscilation = millis();
    oscilation = !oscilation;

    // Do something when Button 3 pressed
    spinRate = spinRate * (-1);
    stateUpdate = true;
    myStepper.setSpeed(spinRate);
    RemoteXY.value_01 = spinRate;

    Serial.print("Oscilation is ");
    if (oscilation) Serial.println("on");
    else Serial.println("off");
  }

  if (RemoteXY.pushSwitch_01 != 0 && !onOffState) {
    // Do something when Button 3 pressed
    onOffState = !onOffState;
    digitalWrite(ENAB, onOffState);
    Serial.println("Motor is on");
    digitalWrite(25, LOW);
  }
  if (RemoteXY.pushSwitch_01 == 0 && onOffState) {
    onOffState = !onOffState;
    digitalWrite(ENAB, onOffState);
    Serial.println("Motor is off");
    digitalWrite(25, HIGH);
  }
  /*  button pressed */


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