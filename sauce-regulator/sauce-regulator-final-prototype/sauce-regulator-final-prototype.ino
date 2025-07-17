//fluid regulator final prototype

#include <Stepper.h>

int stepsPerRevolution=2048;

int motSpeed=2; //rpm

Stepper myStepper(stepsPerRevolution,8,10,9,11);

int dt=2500;

int baud=9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  myStepper.setSpeed(motSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  myStepper.step(-stepsPerRevolution/2);
  delay(dt);
  myStepper.step(stepsPerRevolution/2);
  delay(dt);
}
