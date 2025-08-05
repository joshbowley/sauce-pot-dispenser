//pot release – v5 (M10 same direction as M8)

//=====================================================
//INCLUDES:
#include <AccelStepper.h>

//=====================================================
//PIN ALLOCATION:

//m7 pins (claws open/close)
#define M7_IN1  28
#define M7_IN2  30
#define M7_IN3  32
#define M7_IN4  34

//m8 pins (claws lower/raise)
#define M8_IN1  44
#define M8_IN2  42
#define M8_IN3  40
#define M8_IN4  38

//m9 pins (box turn)
#define M9_IN1  52
#define M9_IN2  50
#define M9_IN3  48
#define M9_IN4  46

//m10 pins (claws lower/raise - sync with m8)
#define M10_IN1 17
#define M10_IN2 16
#define M10_IN3 15
#define M10_IN4 14

//=====================================================
//CREATE STEPPER OBJECTS:

AccelStepper M7(AccelStepper::FULL4WIRE, M7_IN1, M7_IN3, M7_IN2, M7_IN4);
AccelStepper M8(AccelStepper::FULL4WIRE, M8_IN1, M8_IN3, M8_IN2, M8_IN4);
AccelStepper M9(AccelStepper::FULL4WIRE, M9_IN1, M9_IN3, M9_IN2, M9_IN4);
AccelStepper M10(AccelStepper::FULL4WIRE, M10_IN1, M10_IN3, M10_IN2, M10_IN4);

//=====================================================
//MOTOR SETTINGS:

const int stepsPerRevolution = 2048;
const int stepsM8 = stepsPerRevolution * 4;           //m8 gear ratio 4:1
const int stepsM9 = stepsPerRevolution * 4;           //m9 gear ratio 4:1

const float rpm = 2.0;                                //revolutions/minute
const float motSpeed = rpm * stepsPerRevolution / 60.0;
const float motAccel = 50.0;                          //steps/sec²

const int stepDelay = 1000;                           //pause between steps (ms)

//=====================================================
//MOVEMENT SEQUENCE:

struct moveStep {
  AccelStepper* motor;
  int steps;
  const char* name;
};

moveStep moveSequence[] = {
  { &M8, -stepsM8 * 10, "M8" },             //lower claws
  { &M7,  stepsPerRevolution / 16, "M7" },            //close claws
  { &M8,  stepsM8 * 10, "M8" },                       //raise claws
  { &M9,  stepsM9 / 4, "M9" },                        //rotate box cw
  { &M7, -stepsPerRevolution / 8, "M7" },             //open claws
  { &M9, -stepsM9 / 4, "M9" }                         //rotate box ccw
};

const int numSteps = sizeof(moveSequence) / sizeof(moveSequence[0]);
int stepIndex = 0;
bool stepStarted = false;
unsigned long lastStepTime = 0;
bool sequenceComplete = false;

//=====================================================
//HELPER: motor setup
void setupStepper(AccelStepper& motor) {
  motor.setMaxSpeed(motSpeed);
  motor.setAcceleration(motAccel);
  motor.setCurrentPosition(0);
}

//=====================================================
//SETUP:

void setup() {
  Serial.begin(9600);
  Serial.println("Pot release starting...");

  setupStepper(M7);
  setupStepper(M8);
  setupStepper(M9);
  setupStepper(M10);  // m10 setup
}

//=====================================================
//LOOP:

void loop() {
  if (sequenceComplete) return;

  AccelStepper* motor = moveSequence[stepIndex].motor;
  int steps = moveSequence[stepIndex].steps;

  //start step
  if (!stepStarted) {
    motor->moveTo(motor->currentPosition() + steps);

    //sync M10 with M8 (same direction)
    if (motor == &M8) {
      M10.moveTo(M10.currentPosition() + steps);
    }

    stepStarted = true;
    Serial.print("step "); Serial.print(stepIndex);
    Serial.print(": motor "); Serial.print(moveSequence[stepIndex].name);
    Serial.print(" → steps "); Serial.println(steps);
  }

  //run motors
  motor->run();
  M10.run();  // always run M10 to ensure stepping continues

  //check complete
  bool stepComplete = motor->distanceToGo() == 0;
  if (motor == &M8) {
    stepComplete = stepComplete && (M10.distanceToGo() == 0);
  }

  if (stepStarted && stepComplete) {
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(stepDelay);
  }

  //end of sequence
  if (stepIndex >= numSteps) {
    Serial.println("Pot release complete.");
    sequenceComplete = true;
  }
}
