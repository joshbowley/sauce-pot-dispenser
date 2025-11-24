//fluid regulator v3 

#include <AccelStepper.h> 

//create AccelStepper objects: M3 and M4

AccelStepper M3(AccelStepper::FULL4WIRE, 8, 10, 9, 11); 

AccelStepper M4(AccelStepper::FULL4WIRE, 3, 5, 4, 6);

const int stepsPerRevolution = 2048;
const float rpm      = 5.0; 
const float motSpeed = rpm * stepsPerRevolution / 60.0; // steps/sec
const float motAccel = 100.0;                           // steps/sec²
const int dt         = 2000;                            // ms between cycles


void setup() {
  //M3 setup
  M3.setMaxSpeed(motSpeed);
  M3.setAcceleration(motAccel);
  M3.setCurrentPosition(-stepsPerRevolution / 2); //set zero position
  M3.moveTo(stepsPerRevolution / 2);

  //M4 setup
  M4.setMaxSpeed(motSpeed);
  M4.setAcceleration(motAccel);
  M4.setCurrentPosition(stepsPerRevolution / 2); //set zero position
  M4.moveTo(-stepsPerRevolution / 2);
}

void moveMotor(AccelStepper & motor, int steps) {
  motor.move(steps);
  while (motor.distanceToGo() != 0) { 
    motor.run();
  }
}



void loop() {
  if (M3.distanceToGo() != 0 && M4.distanceToGo() != 0){
    M3.run();
    M4.run();
  }
  else {
    delay(dt);
    //turn back 
    M3.moveTo(M3.currentPosition() * -1);
    M4.moveTo(M4.currentPosition() * -1);
  }
}
