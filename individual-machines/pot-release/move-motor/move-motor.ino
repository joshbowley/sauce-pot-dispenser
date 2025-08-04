#include <Stepper.h>

int stepsPerRevolution=2048;

int motSpeed=2; //rpm


//M7 pins (claws open/close)
#define M7_IN1  5
#define M7_IN2  4
#define M7_IN3  3
#define M7_IN4  2

//M8 pins (claws lower/raise)
#define M8_IN1  9
#define M8_IN2  8
#define M8_IN3  7
#define M8_IN4  6

//M9 pins (box turn)
#define M9_IN1  13
#define M9_IN2  12
#define M9_IN3  11
#define M9_IN4  10


//M7, M8 & M9: stepper objects
Stepper M7(stepsPerRevolution, M7_IN1, M7_IN3, M7_IN2, M7_IN4);
Stepper M8(stepsPerRevolution, M8_IN1, M8_IN3, M8_IN2, M8_IN4);
Stepper M9(stepsPerRevolution, M9_IN1, M9_IN3, M9_IN2, M9_IN4);


int dt=5000;

void setup() {
  // put your setup code here, to run once:
  M7.setSpeed(motSpeed);
  M8.setSpeed(motSpeed);
  M9.setSpeed(motSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  M8.step(stepsPerRevolution);
}
