#include <AccelStepper.h>

//M5
AccelStepper M6(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

//M6
AccelStepper M5(AccelStepper::FULL4WIRE, 3, 5, 4, 6);

//define variables for 28BJ-48 motors: 
const int stepsPerRevolution = 2048;
const int stepsM5 = stepsPerRevolution * 9; //M5 is connected to a gear ratio of 9:1
const float rpm = 5.0; 
const float motSpeed = rpm * stepsPerRevolution / 60.0; //steps per second
const float motAccel = 100.0; //steps per sec^2
const int dt = 1000; //delay time between moves (ms) 

// Array of move steps (same sequence as original code)
int moveSequence[][2] = {
  {-stepsPerRevolution / 4,  0},     // M5 up half
  { 0,  stepsPerRevolution / 4},     // M6 up half
  { stepsM5 / 4,  0},     // M5 down half
  { 0,  stepsPerRevolution / 4},     // M6 up half
  {-stepsM5 / 4,  0},     // M5 up half
  { 0, -stepsPerRevolution / 2},     // M6 down full
  {-stepsM5 / 4,  0},     // M5 up half
  { stepsM5 / 2,  0},     // M5 down full
};

int stepIndex = 0;
unsigned long lastStepTime = 0;

void setup() {
  M5.setMaxSpeed(motSpeed);
  M5.setAcceleration(motAccel);


  M6.setMaxSpeed(motSpeed);
  M6.setAcceleration(motAccel);
}

void loop() {
  // Move both motors toward their target
  M5.run();
  M6.run();

  // Check if both motors are done
  if (!M5.isRunning() && !M6.isRunning()) {
    // Check if delay after last move has passed
    if (millis() - lastStepTime >= dt) {
      // Load next movement step
      M5.move(moveSequence[stepIndex][0]);
      M6.move(moveSequence[stepIndex][1]);

      M5.run(); // kickstart
      M6.run();

      stepIndex++;
      if (stepIndex >= sizeof(moveSequence) / sizeof(moveSequence[0])) {
        stepIndex = 0; // loop
      }

      lastStepTime = millis(); // reset delay timer
    }
  }
}
