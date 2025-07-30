// conveyor belt prototype v2 (corrected)

// include accelstepper library for non-blocking, smooth stepper motion
#include <AccelStepper.h>

// create AccelStepper object
AccelStepper myStepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

int stepsPerRevolution = 2048;
float rpm = 5.0;
float motSpeed = rpm * stepsPerRevolution / 60.0; // ~170 steps/s
float motAccel = 100.0;
int dt = 2000;
int baud = 9600;
unsigned long lastMoveTime = 0;
bool moving = false;

void setup() {
  Serial.begin(baud);
  myStepper.setMaxSpeed(motSpeed);
  myStepper.setAcceleration(motAccel);
  Serial.println("AccelStepper conveyor belt setup has completed");
}

void loop() {
  // Call run() EVERY LOOP to allow AccelStepper to process movement
  myStepper.run();

  if ((!moving) && (millis() - lastMoveTime > dt)) {
    myStepper.move(stepsPerRevolution / 2); // half revolution
    moving = true;
    Serial.println("Starting conveyor belt movement.");
  }

  if (moving && myStepper.distanceToGo() == 0) {
    moving = false;
    lastMoveTime = millis();
    Serial.println("Conveyor belt movement complete.");
  }
}
