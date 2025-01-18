// Include Libraries
#include <AccelStepper.h>
//#include <BLEDevice.h>

#include <ArduinoOTA.h>  // For enabling over the air updates
#include <WiFi.h>        // or connecting ESP32 to WiFi

#define REMOTEXY_MODE__ESP32CORE_BLE

// Microcontroller - ESP32 WROOM and stepper Driver TMC2208
#define ENAB 32  // pin for EN 25 32 33
#define MS1 33   // pin for MS1
#define MS2 25   // pin for MS2

const int dirPin = 27;
const int stepPin = 26;
// #define STEPPIN 26  // pin for STEP
// #define DIRPIN 27   // pin for DIR


//#define REMOTEXY_BLUETOOTH_NAME "Swimming_robot"  // name for Bluetooth device

//#include <RemoteXY.h>

// Set WiFi Credentials
//const char* ssid = "Adil_iPhone";   // Change to your WiFi Network name
//const char* password = "87654321";  // Change to you password

// RemoteXY GUI configuration
// #pragma pack(push, 1)
// uint8_t RemoteXY_CONF[] =  // 93 bytes
//   { 255, 4, 0, 2, 0, 86, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 5, 0,
//     1, 29, 80, 48, 24, 1, 49, 31, 83, 112, 101, 101, 100, 32, 43, 0, 1, 29, 116, 49,
//     24, 1, 175, 31, 83, 112, 101, 101, 100, 32, 45, 0, 1, 39, 159, 31, 24, 2, 120, 31,
//     82, 101, 118, 101, 114, 115, 101, 0, 67, 20, 41, 69, 29, 85, 2, 26, 10, 42, 7, 26,
//     26, 48, 4, 26, 31, 79, 78, 0, 31, 79, 70, 70, 0 };

// this structure defines all the variables and events of your control interface
// struct {

//   // input variables
uint8_t speedUp;        // =1 if button pressed, else =0
uint8_t speedDown;      // =1 if button pressed, else =0
uint8_t reverse;        // =1 if button pressed, else =0
uint8_t pushSwitch_01;  // =1 if state is ON, else =0

//   // output variables
int16_t value_01;  // -32768 .. +32767

//   // other variable
uint8_t connect_flag;  // =1 if wire connected, else =0

// } RemoteXY;
// #pragma pack(pop)

// Speed control variables
int spinRate = 500;  // in RPM
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

// bool speedUp = LOW;
bool speedUpState = LOW;
long currentSpeedUp = 0;

// Creates an instance - Pick the version you want to use and un-comment it. That's the only required change.
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  // pinMode(ENAB, OUTPUT);
  // pinMode(MS1, OUTPUT);
  // pinMode(MS2, OUTPUT);
  // pinMode(STEPPIN, OUTPUT);
  // pinMode(DIRPIN, OUTPUT);


  // digitalWrite(ENAB, HIGH);  // Motor is Off by default
  // digitalWrite(MS1, HIGH);
  // digitalWrite(MS2, HIGH);

  // Connect to WiFi
 // WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode
  // Wait for WiFi to connect
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
  // ArduinoOTA.begin();  // Starts OTA

  stepper.setMaxSpeed(5000);
  stepper.setSpeed(500);

  // RemoteXY_Init();

  //  Serial.begin(9600);
  // digitalWrite(ENAB, LOW);       // Motor is Off by default
  // myStepper.setMaxSpeed(10000);  // this limits the value of setSpeed(). Raise it if you like.
  // myStepper.setSpeed(1000);       // runSpeed() will run the motor at this speed - set it to whatever you like.
  // myStepper.moveTo(10000);
  // myStepper.moveTo(-10000);
  // myStepper.runSpeed();  // This will run the motor forever.
}

void loop() {
 // ArduinoOTA.handle();
  //  RemoteXY_Handler();
  stepper.runSpeed();

  // if (speedUpState) {
  //   if (millis() - currentSpeedUp >= 1000) speedUpState = LOW;
  // }
  // if (RemoteXY.pushSwitch_01 != 0 && !onOffState) {
  //   // Do something when Button 3 pressed
  //   onOffState = !onOffState;
  //   digitalWrite(ENAB, onOffState);
  //   //  Serial.println("Motor is on");
  // }
  // if (RemoteXY.pushSwitch_01 == 0 && onOffState) {
  //   onOffState = !onOffState;
  //   digitalWrite(ENAB, onOffState);
  //   //  Serial.println("Motor is off");
  // }

  // myStepper.runSpeed();

  // if (stateUpdate) {

  //   // Serial.print("SpinRate: ");
  //   // Serial.print(spinRate);
  //   // Serial.print(" Units |  Steps: ");
  //   // Serial.print(steps);
  //   // Serial.print("\n--------------------------------------------\n");
  //   stateUpdate = false;
  // }
}
