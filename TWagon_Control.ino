/*
Drive setup Based off of examples from http://www.hessmer.org/blog/2013/12/28/ibt-2-h-bridge-with-arduino/
Drive Motor Setup:
// Connection to the Motor Driver boards:
// Drive PWM to Arduino pin 5
// R_EN, L_EN, VCC to Arduino 5V pin
// GND to Arduino GND
// R_IS and L_IS not connected

Steering Setup: 
// Connections:
// Arduino Pin 9 (PWM)
// Arduino 5V (power)
// Arduino GND (GND)
*/

// PWM connected to pin 5
#define PWM 5
 
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
int maxAngle = 25; // max turning angle of wheels
long distance = 0; // Distance between cart and target

Servo steerServo; // Servo object to control steering servo
int steerAngle = 142; // Angle sent to steering servo

void setup() {
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set PWM pin 5 as output
  pinMode(PWM, OUTPUT);

  // Steering servo controlled by pin #9
  steerServo.attach(9, 1000, 2000);
  steerServo.write(steerAngle);
}

void loop() {
  /* Read coordinate from R Pi
   * Coordinate should be sent from Rpi in the 
   * format of "x#y#" where # = int  
   */
  if(Serial.available() > 0){
    // Go to beginning of coordinate
    if(Serial.find('x') == true){
      // Read coordinates and store into var x and y
      x = Serial.parseInt();
      if(Serial.find('y') == true){
        y = Serial.parseInt();
        Serial.print("x: ");
        Serial.println(x);
        Serial.print("Y: ");
        Serial.println(y);
      }
      else{
        Serial.print("Error\n");
      }
    }
  }

    // Calculate distance and angle
    distance = sqrt(sq(x) + sq(y));
    angle = (atan2(y,x) * 180 / M_PI) - 90;
    angle = -angle;
    if(distance == 0){
      angle = 0;
    }
    else if(angle < -maxAngle){
      angle = -maxAngle;
    }
    else if(angle > maxAngle){
      angle = maxAngle;
    }

    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Angle: ");
    Serial.println(angle); 
    Serial.println();
    

    if(distance >= 75){
      motorPwm = 90;
    }
    else{
      motorPwm = 0;
      angle = 0;
    }
    
    steerAngle = 142 + angle; // servo middle=150
    steerServo.write(steerAngle);

   
    // Write PWM to controller
    analogWrite(PWM, motorPwm);
}
