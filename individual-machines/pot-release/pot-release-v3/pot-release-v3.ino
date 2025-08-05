//pot distributor v3 – standardised format for m1

#include <AccelStepper.h>

//m1 pins
#define M1_IN1  6
#define M1_IN2  7
#define M1_IN3  8
#define M1_IN4  9

//m1: stepper object
AccelStepper M1(AccelStepper::FULL4WIRE, M1_IN1, M1_IN3, M1_IN2, M1_IN4);

//motor parameters setup
const int stepsPerRevolution = 2048;
const float rpm              = 5.0;
const float motSpeed         = rpm * stepsPerRevolution / 60.0;  //steps/sec
const float motAccel         = 100.0;                            //steps/sec²
const int dt                 = 2000;                             //ms between cycles

//datastructure for movement information
struct moveStep {
  AccelStepper* motor;
  int steps;
  const char* name;
};

//movement sequence
moveStep moveSequence[] = {
  { &M1, -stepsPerRevolution, "M1" },
  { &M1, -stepsPerRevolution, "M1" }
};

//runtime tracking
int stepIndex = 0;
bool stepStarted = false;
unsigned long lastStepTime = 0;
const int numSteps = sizeof(moveSequence) / sizeof(moveSequence[0]);

//setup helper
void setupStepper(AccelStepper & motor) {
  motor.setMaxSpeed(motSpeed);
  motor.setAcceleration(motAccel);
  motor.setCurrentPosition(0);
}

//baud rate
const int baud = 9600;

void setup() {
  Serial.begin(baud);
  Serial.println("pot distributor starting...");

  setupStepper(M1);
}

void loop() {
  moveStep current = moveSequence[stepIndex];

  //start move
  if (!stepStarted) {
    current.motor->moveTo(current.motor->currentPosition() + current.steps);
    stepStarted = true;
    Serial.print("step "); Serial.print(stepIndex);
    Serial.print(": motor "); Serial.print(current.name);
    Serial.print(" → steps "); Serial.println(current.steps);
  }

  //run motor
  current.motor->run();

  //check if complete
  if (stepStarted && current.motor->distanceToGo() == 0) {
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(dt);
  }

  //loop sequence
  if (stepIndex >= numSteps) {
    stepIndex = 0;
    Serial.println("pot distributor looped");
  }
}
