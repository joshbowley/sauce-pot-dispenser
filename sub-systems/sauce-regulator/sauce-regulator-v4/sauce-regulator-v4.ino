//fluid regulator v5 – standardised format with dual-motor steps

#include <AccelStepper.h>

//m3 pins
#define M3_IN1  2
#define M3_IN2  3
#define M3_IN3  4                      
#define M3_IN4  5

//m4 pins
#define M4_IN1  53
#define M4_IN2  51
#define M4_IN3  49
#define M4_IN4  47

//m3 and m4: stepper objects
AccelStepper M3(AccelStepper::FULL4WIRE, M3_IN1, M3_IN3, M3_IN2, M3_IN4);
AccelStepper M4(AccelStepper::FULL4WIRE, M4_IN1, M4_IN3, M4_IN2, M4_IN4);

//motor parameters setup
const int stepsPerRevolution = 2048;
const float rpm              = 5.0;
const float motSpeed         = rpm * stepsPerRevolution / 60.0;  //steps/sec
const float motAccel         = 100.0;                            //steps/sec²
const int dt                 = 2000;                             //ms between cycles

//datastructure for dual-motor steps
struct moveStep {
  AccelStepper* motorA;
  int stepsA;
  const char* nameA;

  AccelStepper* motorB;
  int stepsB;
  const char* nameB;
};

//movement sequence
moveStep moveSequence[] = {
  { &M3,  stepsPerRevolution,  "M3", &M4, -stepsPerRevolution,  "M4" },
  { &M3, -stepsPerRevolution,  "M3", &M4,  stepsPerRevolution,  "M4" }
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
  Serial.println("fluid regulator starting...");

  setupStepper(M3);
  setupStepper(M4);
}

void loop() {
  moveStep current = moveSequence[stepIndex];

  //start move
  if (!stepStarted) {
    current.motorA->moveTo(current.motorA->currentPosition() + current.stepsA);
    current.motorB->moveTo(current.motorB->currentPosition() + current.stepsB);
    stepStarted = true;

    Serial.print("step "); Serial.print(stepIndex); Serial.println(":");
    Serial.print("  motor "); Serial.print(current.nameA); Serial.print(" → "); Serial.println(current.stepsA);
    Serial.print("  motor "); Serial.print(current.nameB); Serial.print(" → "); Serial.println(current.stepsB);
  }

  //run both motors
  current.motorA->run();
  current.motorB->run();

  //check if both complete
  if (stepStarted &&
      current.motorA->distanceToGo() == 0 &&
      current.motorB->distanceToGo() == 0) {
    
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(dt);
  }

  //loop sequence
  if (stepIndex >= numSteps) {
    stepIndex = 0;
    Serial.println("fluid regulator looped");
  }
}
