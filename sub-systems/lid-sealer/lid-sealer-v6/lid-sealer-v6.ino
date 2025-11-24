//lid sealer v6 – standardised format for m5 and m6

#include <AccelStepper.h>

//m5 pins (vertical motion with gear 9:1)
#define M5_IN1  13
#define M5_IN2  12
#define M5_IN3  11
#define M5_IN4  9

//m6 pins (rotation)
#define M6_IN1  39
#define M6_IN2  41
#define M6_IN3  43
#define M6_IN4  45

//m5 and m6: stepper objects
AccelStepper M5(AccelStepper::FULL4WIRE, M5_IN1, M5_IN3, M5_IN2, M5_IN4);
AccelStepper M6(AccelStepper::FULL4WIRE, M6_IN1, M6_IN3, M6_IN2, M6_IN4);

//motor parameters setup
const int stepsPerRevolution = 2048;
const int stepsM6            = stepsPerRevolution * 9;            //m6 gear 9:1
const float rpm              = 5.0;
const float motSpeed         = rpm * stepsPerRevolution / 60.0;   //steps/sec
const float motAccel         = 100.0;                             //steps/sec²
const int dt                 = 1000;                              //ms between steps

//datastructure for movement information
struct moveStep {
  AccelStepper* motor;
  int steps;
  const char* name;
};

//movement sequence
moveStep moveSequence[] = {
  { &M6, -stepsM6 / 4, "M6" },               //m6 up 1/4
  { &M5,  stepsPerRevolution / 4, "M5" },    //m5 turn 1/4 cw
  { &M6,  stepsM6 / 4, "M6" },               //m6 down 1/4
  { &M5,  stepsPerRevolution / 4, "M5" },    //m5 turn another 1/4 cw
  { &M6, -stepsM6 / 4, "M6" },               //m6 up 1/4
  { &M5, -stepsPerRevolution / 2, "M5" },    //m5 turn back 1/2 ccw
  { &M6, -stepsM6 / 4, "M6" },               //m6 up 1/4
  { &M6,  stepsM6 / 2, "M6" }                //m6 down 1/2
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
  Serial.println("lid sealer starting...");

  setupStepper(M5);
  setupStepper(M6);
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

  //check complete
  if (stepStarted && motor->distanceToGo() == 0) {
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(dt);
  }

  //end or loop
  if (stepIndex >= numSteps) {
    stepIndex = 0;
    Serial.println("lid sealer looped");
  }
}
