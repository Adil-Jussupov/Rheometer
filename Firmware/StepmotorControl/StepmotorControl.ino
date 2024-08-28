/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins
#define stepPin D4
#define dirPin D5
float spinRate = 1;
bool switchMot = LOW;
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
long prevtime = 0;
bool status = HIGH;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(9600);
  inputString.reserve(200);
  digitalWrite(dirPin, true);  // Enables the motor to move in a particular direction
}
void loop() {
  if (stringComplete) {
    //Serial.println(switchMot);
    Serial.println(spinRate);
    digitalWrite(dirPin, switchMot);  // Enables the motor to move in a particular direction
    stringComplete = false;
  }

  // Makes 200 pulses for making one full cycle rotation
  if ((micros() - prevtime) > 700) {
    digitalWrite(stepPin, status);
    prevtime = micros();
    status = !status;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '2') {
      spinRate += 0.1;
      stringComplete = true;
      break;
    }
    if (inChar == '1') {
      spinRate -= 0.1;
      stringComplete = true;
      break;
    }
    if (inChar == '4') {
      spinRate += 1.0;
      stringComplete = true;
    }
    if (inChar == '3') {
      spinRate -= 1.0;
      stringComplete = true;
      break;
    }
    if (inChar == 'q') {
      switchMot = !switchMot;
      stringComplete = true;
      break;
    }
    if (inChar == '\n') {
      break;
    }
  }
}