//pot distributor v2

//Requirements: 
//motor moves one full negative rotation, then pause, and repeat.

#include <AccelStepper.h>

//create a stepper object - labelled M1
AccelStepper M1(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

const int stepsPerRevolution = 2048;  //for 28BYJ-48
const float rpm = 5.0;
const float motSpeed = rpm * stepsPerRevolution / 60.0; //steps per second
const float motAccel = 100.0;  //steps per sec²
const int dt = 2000;  //delay time between moves (ms)


void setup() {
  // put your setup code here, to run once:
  M1.setMaxSpeed(motSpeed);
  M1.setAcceleration(motAccel);
  M1.setCurrentPosition(0); //set zero position
  M1.moveTo(-stepsPerRevolution); //negative full turn
}

void loop() {
  // put your main code here, to run repeatedly:
  if (M1.distanceToGo() != 0 ){ 
    M1.run();
  }
  else {
    delay(dt); //pause
    //go again and add another negative full rotation
    M1.moveTo(M1.currentPosition() - stepsPerRevolution);
  }
}
