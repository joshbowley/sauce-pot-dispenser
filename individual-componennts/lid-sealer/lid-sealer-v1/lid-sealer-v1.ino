//lid sealer v1

#include <Stepper.h>

int stepsPerRevolution=2048;

int stepsM5=18432; //gear ratio of M5 is 9:1

int motSpeed=10; //rpm

//create stepper objects for M5 and M6
Stepper M6(stepsPerRevolution,8,10,9,11);
Stepper M5(stepsPerRevolution,4,6,5,7);

int baud=9600;

int dt1=100;
int dt2=5000;

void setup() {
  Serial.begin(baud);
  M5.setSpeed(motSpeed);
  M6.setSpeed(motSpeed);
}

void loop() {
  M5.step(-stepsM5/4);
  M6.step(stepsPerRevolution/4);
  M5.step(stepsM5/4);
  M6.step(stepsPerRevolution/4);
  M5.step(-stepsM5/4);
  M6.step(-stepsPerRevolution/2);
  M5.step(-stepsM5/4);
  delay(dt1);
  M5.step(stepsM5/4);
  delay(dt2);
}
