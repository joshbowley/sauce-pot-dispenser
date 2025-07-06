//pot distributor prototype 1

#include <Stepper.h>

int stepsPerRevolution=2048;

int motSpeed=10; //rpm

Stepper myStepper(stepsPerRevolution,8,10,9,11);

int dt=500;

int baud=9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  myStepper.setSpeed(motSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  myStepper.step(-stepsPerRevolution);
}
