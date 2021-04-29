#include <math.h>

long x = 0;  // X coordinate of target in centimeters
long y = 0;  // Y coordinate of target in centimeters
int angle = 0;  // turn angle of wheels
int maxAngle = 45; // max turning angle of wheels
long distance = 0; // Distance between cart and target

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

}

void loop() {
  /* Read coordinate from R Pi
   * Coordinate should be sent from Rpi in the 
   * format of "x#y#" where # = int  
   */
  if(Serial.available()){
    // Go to beginning of coordinate
    if(Serial.find('x') == true){
      // Read coordinates and store into var x and y
      x = Serial.parseInt();
      if(Serial.find('y') == true){
        y = Serial.parseInt();
      }
      else{
        Serial.print("Error\n");
      }
    // Calculate distance and angle
    distance = sqrt(sq(x) + sq(y));
    angle = (atan2(y,x) * 180 / M_PI) - 90;
    if(distance == 0){
      angle = 0;
    }
    else if(angle < -maxAngle){
      angle = -maxAngle;
    }
    else if(angle > maxAngle){
      angle = maxAngle;
    }
    Serial.print("x: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Angle: ");
    Serial.println(angle);
    }
  }
}
