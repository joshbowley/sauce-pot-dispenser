#include <AccelStepper.h>

// Correct constructor for your wiring:
AccelStepper stepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(100);       // very conservative
  stepper.setAcceleration(50);    // very conservative
  stepper.moveTo(4096);           // 2 full revolutions
  Serial.println("AccelStepper test: setup completed, starting movement");
}

void loop() {
  if (stepper.distanceToGo() != 0) {
    stepper.run();
  } else {
    Serial.println("Move complete. Reversing direction.");
    delay(1000);  // pause briefly
    stepper.moveTo(-stepper.currentPosition()); // move back to start
  }
}
