//conveyor belt v3- using accel Stepper library

//Requirements: 
//motor move half positive rotation, then pause, and repeat. 

#include <AccelStepper.h>

//create stepper object - labelled M2
AccelStepper M2(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

const int stepsPerRevolution = 2048;  //for 28BYJ-48
const float rpm = 5.0;
const float motSpeed = rpm * stepsPerRevolution / 60.0; //steps per second
const float motAccel = 100.0;  //steps per sec²
const int dt = 2000;  //delay time between moves (ms)


void setup() {
  M2.setMaxSpeed(motSpeed);
  M2.setAcceleration(motAccel);
  M2.setCurrentPosition(0); //set zero position
  M2.moveTo(stepsPerRevolution / 2); //half turn
}

void loop() {
  if (M2.distanceToGo() != 0) {
    M2.run();
  }
  else {
    delay(dt); //pause
    //go again and add another half turn to target
    M2.moveTo(M2.currentPosition() + stepsPerRevolution / 2);
  }

}
