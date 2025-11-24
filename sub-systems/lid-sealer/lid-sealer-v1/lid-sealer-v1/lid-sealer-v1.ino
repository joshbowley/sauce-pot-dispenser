//lid sealer v1- using stepper library

#include <Stepper.h>

const int stepsPerRevolution=2028;

const int motSpeed=10; //rpm

Stepper M5(stepsPerRevolution, 8, 10, 9, 11);

Stepper M6(stepsPerRevolution, 3, 5, 4, 6);

const int dt=1000;

void setup() {
  M5.setSpeed(motSpeed);
  M6.setSpeed(motSpeed);
}

void loop() {
  //up half
  M5.step(-stepsPerRevolution / 4);
  //up half
  M6.step(stepsPerRevolution / 4);
  //down half
  M5.step(stepsPerRevolution / 4);
  //up half
  M6.step(stepsPerRevolution / 4);
  //up half
  M5.step(-stepsPerRevolution / 4);
  //down full
  M6.step(-stepsPerRevolution / 2);
  //up half
  M5.step(-stepsPerRevolution / 4);
  //down full
  M5.step(stepsPerRevolution / 2);

  delay(dt);
  }
