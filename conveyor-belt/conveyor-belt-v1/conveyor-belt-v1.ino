//coveyor belt prototype 1

#include <Stepper.h>

int stepsPerRevolution=2048;

int motSpeed=2; //rpm

Stepper myStepper(stepsPerRevolution,8,10,9,11);

int dt=5000;

int baud=9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  myStepper.setSpeed(motSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  myStepper.step(stepsPerRevolution/2); //half turn of motor should be 1/8 turn of belt due to gears 
  delay(dt);
}
