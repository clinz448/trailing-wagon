 /*
Based off of examples from https://dronebotworkshop.com/rotary-encoders-arduino/ and http://www.hessmer.org/blog/2013/12/28/ibt-2-h-bridge-with-arduino/
Setup:
// Connection to Arduino:
// Pin 3 to encoder output A
// Pin 5 is PWM output
// A0 to a potentiometer

// Connection to the Motor Driver board:
// RPWM to Arduino pin 5(PWM)
// LPWM to Arduino pin 6(PWM) (I'm not sending anything to this though)
// R_EN, L_EN, VCC to Arduino 5V pin
// GND to Arduino GND
// R_IS and L_IS not connected
*/
 
// Motor encoder output pulse per rotation
#define ENC_COUNT_REV 24 //might be wrong

// RPWM connected to pin 5
#define PWM 5

// encoder in to pin 3 (for interrupts)(left wheel)
#define ENC_IN_R 3

//encoder in to pin 2 (for interrupts) (right wheel)
#define ENC_IN_L 2

// Pulse count from encoder R (left wheel)
volatile long encoderValueR = 0;

// Pulse count from encoder L
volatile long encoderValueL = 0;
 
// One-second interval for measurements
int interval = 1000;
 
// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;
 
// Variable for RPM measuerment R
int rpmR = 0;

// Variable for RPM measurement L
int rpmL = 0;
 
// Variable for PWM motor speed output
int motorPwm = 0;

/* Steering Variables
 *  
 *  
 */
#include <math.h>
#include <Servo.h>

long x = 0;  // X coordinate of target in centimeters
long y = 0;  // Y coordinate of target in centimeters
int angle = 0;  // turn angle of wheels
int maxAngle = 45; // max turning angle of wheels
long distance = 0; // Distance between cart and target

Servo steerServo; // Servo object to control steering servo
int steerAngle = 0; // Angle sent to steering servo
 
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set encoder as input with internal pullup  
  pinMode(3, INPUT_PULLUP); 

  pinMode(2, INPUT_PULLUP);
  // Set PWM pin 5 as output
  pinMode(PWM, OUTPUT);
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(ENC_IN_R), updateEncoderR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_L), updateEncoderL, RISING);
  
  // Setup initial values for timer
  previousMillis = millis();

  // Steering servo controlled by pin #9, start at -45 degrees
  steerServo.attach(9, 1000, 2000);
  steerAngle = 117;
  steerServo.write(117);
}
 
void loop()
{
  
    // Control motor with potentiometer
    motorPwm = map(analogRead(A0), 0, 1023, 0, 255);
    
    // Write PWM to controller
    analogWrite(PWM, motorPwm);
  
    /*// Update RPM value every second
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
 
 
    // Calculate RPM R
    rpmR = (float)(encoderValueR/ ENC_COUNT_REV);

    // Calculate RPM L
    rpmL = (float)(encoderValueL/ ENC_COUNT_REV);
    
    encoderValueR = 0;
    encoderValueL = 0;
  }

  */

  if(motorPwm == 255){
    currentMillis = millis();
  
    if(currentMillis - previousMillis > 6000){
      switch(steerAngle){
        case 163:
          steerAngle = 117;
          //Serial.println("Changing to 117");
          break;
        case 117:
          steerAngle = 163;
          //Serial.println("Changing to 163");
          break;
      }
      previousMillis = currentMillis;
      steerServo.write(steerAngle);
    }
  }
}
 
void updateEncoderR()
{
  // Increment value for each pulse from encoder
  encoderValueR++;
}
void updateEncoderL()
{
  encoderValueL++;
}
