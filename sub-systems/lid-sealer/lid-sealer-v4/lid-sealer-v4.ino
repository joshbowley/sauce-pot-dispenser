#include <AccelStepper.h>

// M5 and M6 wired with correct coil sequence
AccelStepper M5(AccelStepper::FULL4WIRE, 11, 10, 9, 8);  // IN1-IN4
AccelStepper M6(AccelStepper::FULL4WIRE, 6, 5, 4, 3);    // IN1-IN4

const int stepsPerRevolution = 2048;
const float rpm = 5.0;
const float motSpeed = rpm * stepsPerRevolution / 60.0; // steps/sec
const float motAccel = 100.0;
const int dt = 1000; // ms delay between moves

// Define movement sequence
int moveSequence[][2] = {
  {-stepsPerRevolution / 4,  0},     // M5 up half
  { 0,  stepsPerRevolution / 4},     // M6 up half
  { stepsPerRevolution / 4,  0},     // M5 down half
  { 0,  stepsPerRevolution / 4},     // M6 up half
  {-stepsPerRevolution / 4,  0},     // M5 up half
  { 0, -stepsPerRevolution / 2},     // M6 down full
  {-stepsPerRevolution / 4,  0},     // M5 up half
  { stepsPerRevolution / 2,  0},     // M5 down full
};

int stepIndex = 0;
unsigned long lastStepTime = 0;

void setup() {
  Serial.begin(9600);

  // Set up both motors
  M5.setMaxSpeed(motSpeed);
  M5.setAcceleration(motAccel);
  M6.setMaxSpeed(motSpeed);
  M6.setAcceleration(motAccel);

  // Optional fix: uncomment if both directions still spin CW
  // M5.setPinsInverted(true, false, false);
  // M6.setPinsInverted(true, false, false);

  Serial.println("Stepper direction test starting...");
}

void loop() {
  // Continuously run motors
  M5.run();
  M6.run();

  // If both motors have finished moving
  if (!M5.isRunning() && !M6.isRunning()) {
    if (millis() - lastStepTime >= dt) {
      // Print movement details
      Serial.print("Step ");
      Serial.print(stepIndex);
      Serial.print(" | M5 move: ");
      Serial.print(moveSequence[stepIndex][0]);
      Serial.print(" | M6 move: ");
      Serial.println(moveSequence[stepIndex][1]);

      // Move motors
      M5.move(moveSequence[stepIndex][0]);
      M6.move(moveSequence[stepIndex][1]);

      // Also print target positions
      Serial.print("M5 target: ");
      Serial.print(M5.targetPosition());
      Serial.print(" | Current: ");
      Serial.println(M5.currentPosition());

      Serial.print("M6 target: ");
      Serial.print(M6.targetPosition());
      Serial.print(" | Current: ");
      Serial.println(M6.currentPosition());

      // Start motion
      M5.run();
      M6.run();

      // Next step
      stepIndex++;
      if (stepIndex >= sizeof(moveSequence) / sizeof(moveSequence[0])) {
        stepIndex = 0;
        Serial.println("=== Sequence looped ===");
      }

      lastStepTime = millis();
    }
  }
}
