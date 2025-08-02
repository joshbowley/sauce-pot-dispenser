//lid sealer v2 - using accelStepper Library for sequential movement of M5 and M6

#include <AccelStepper.h>

//create two Accel stepper objects - labelled M5 and M6: 

//M5
AccelStepper M5(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

//M6
AccelStepper M6(AccelStepper::FULL4WIRE, 3, 4, 5, 6);

//define variables for 28BJ-48 motors: 
const int stepsPerRevolution = 2048;
const float rpm = 5.0; 
const float motSpeed = rpm * stepsPerRevolution / 60.0; //steps per second
const float motAccel = 100.0; //steps per sec^2
const int dt = 1000; //delay time between moves (ms) 


//setup
void setup() {
  //M5 motor setup 
  M5.setMaxSpeed(motSpeed);
  M5.setAcceleration(motAccel);

  //M6 motor setup
  M6.setMaxSpeed(motSpeed);
  M6.setAcceleration(motAccel);
}

//create a function for sequential movement: 
void moveMotor(AccelStepper & motor, int steps) {
  motor.move(steps);
  while (motor.distanceToGo() != 0) {
    motor.run();
  }
}

//loop running movement logic
void loop() {
  //up half
  moveMotor(M5, -stepsPerRevolution / 4);
  //up half
  moveMotor(M6, stepsPerRevolution / 4);
  //down half
  moveMotor(M5, stepsPerRevolution / 4);
  //up half
  moveMotor(M6, stepsPerRevolution / 4);
  //up half
  moveMotor(M5, -stepsPerRevolution / 4);
  //down full
  moveMotor(M6, -stepsPerRevolution / 2);
  //up half
  moveMotor(M5, -stepsPerRevolution / 4);
  //down full
  moveMotor(M5, stepsPerRevolution / 2);

  delay(dt);
}