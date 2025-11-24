//conveyor belt v4 – standardized format

#include <AccelStepper.h>

//m2 pins (conveyor belt)
#define M2_IN1  37
#define M2_IN2  35
#define M2_IN3  33
#define M2_IN4  31

//m2: stepper object
AccelStepper M2(AccelStepper::FULL4WIRE, M2_IN1, M2_IN3, M2_IN2, M2_IN4);

//motor parameters setup
const int stepsPerRevolution = 2048;
const float rpm              = 5.0;
const float motSpeed         = rpm * stepsPerRevolution / 60.0;  //steps/sec
const float motAccel         = 100.0;                            //steps/sec²
const int dt                 = 500;                              //ms between cycles

//datastructure for movement information
struct moveStep {
  AccelStepper* motor;
  int steps;
  const char* name;
};

//movement sequence
moveStep moveSequence[] = {
  { &M2, stepsPerRevolution / 2, "M2" },
  { &M2, stepsPerRevolution / 2, "M2" }
};

//runtime tracking
int stepIndex = 0;
bool stepStarted = false;
unsigned long lastStepTime = 0;
const int numSteps = sizeof(moveSequence) / sizeof(moveSequence[0]);

//simplify setup
void setupStepper(AccelStepper & motor) {
  motor.setMaxSpeed(motSpeed);
  motor.setAcceleration(motAccel);
  motor.setCurrentPosition(0);
}

//baud rate: serial monitor
const int baud = 9600;

void setup() {
  Serial.begin(baud);
  Serial.println("conveyor belt starting...");

  setupStepper(M2);
}

void loop() {
  AccelStepper* motor = moveSequence[stepIndex].motor;
  int steps = moveSequence[stepIndex].steps;

  //begin move
  if (!stepStarted) {
    motor->moveTo(motor->currentPosition() + steps);
    stepStarted = true;
    Serial.print("step "); Serial.print(stepIndex);
    Serial.print(": motor "); Serial.print(moveSequence[stepIndex].name);
    Serial.print(" → steps "); Serial.println(steps);
  }

  //run motor
  motor->run();

  //complete move
  if (stepStarted && motor->distanceToGo() == 0) {
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(dt);
  }

  //loop sequence
  if (stepIndex >= numSteps) {
    stepIndex = 0;
    Serial.println("conveyor belt looped");
  }
}
