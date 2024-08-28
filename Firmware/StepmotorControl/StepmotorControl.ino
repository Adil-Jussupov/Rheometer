/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins
#define stepPin D4
#define dirPin D5

long prevtime = 0;
bool stepFlag = HIGH;
bool dirFlag = HIGH;
long prevTime = 0;
int stepDelay = (1250 * 6) / 1.6;  // initial delay at 1 RPM spinRate
int revolutions = 0;
float spinRate = 0.100;  // in RPM

void setup() {
  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, dirFlag);  // Enables the motor to move in a particular direction
  prevTime = millis();
  stepDelay = stepDelay / spinRate;
}
void loop() {

  // Makes 200 pulses for making one full cycle rotation
  if (revolutions >= 12800){
    Serial.print("SpinRate: "); Serial.print(spinRate);
    Serial.print(" RPM |  Time: "); Serial.print(millis() - prevTime);
    Serial.print(" ms  |  Delay: "); Serial.print(stepDelay); Serial.print("\n--------------------------------------------------------------\n");

    revolutions = 0;
    //dirFlag = !dirFlag; // change direction every revolution
    //digitalWrite(dirPin, dirFlag);
    //digitalWrite(stepPin, true); // release coils in the motor
    delay(3000);

    prevTime = millis();
  }
  if ((micros() - prevtime) > stepDelay) {
    digitalWrite(stepPin, stepFlag);
    prevtime = micros();
    stepFlag = !stepFlag;
    ++revolutions;
  }

}