//pot release v1

//3 axis motion so 3 motors labelled M7, M8, M9

#include <AccelStepper.h>

//M7 pins (claws open/close)
#define M7_IN1  5
#define M7_IN2  4
#define M7_IN3  3
#define M7_IN4  2

//M8 pins (claws lower/raise)
#define M8_IN1  9
#define M8_IN2  8
#define M8_IN3  7
#define M8_IN4  6

//M9 pins (box turn)
#define M9_IN1  13
#define M9_IN2  12
#define M9_IN3  11
#define M9_IN4  10


//M7, M8 & M9: stepper objects
AccelStepper M7(AccelStepper::FULL4WIRE, M7_IN1, M7_IN3, M7_IN2, M7_IN4);
AccelStepper M8(AccelStepper::FULL4WIRE, M8_IN1, M8_IN3, M8_IN2, M8_IN4);
AccelStepper M9(AccelStepper::FULL4WIRE, M9_IN1, M9_IN3, M9_IN2, M9_IN4);


//motor parameters setup
const int stepsPerRevolution = 2048;
const int stepsM9            = stepsPerRevolution * 4;           // M9 is attached to gear ratio of 4:1
const float rpm              = 2.0;                              // revolutions/minute
const float motSpeed         = rpm * stepsPerRevolution / 60.0;  // steps/sec
const float motAccel         = 50.0;                            // steps/sec²
const int dt                 = 1000;                             // ms between cycles
 

//datastructure for movement information
struct moveStep {
  AccelStepper* motor; 
  int steps;
  const char* name; //label for serial printing (debugging)
};


//movement sequence (signed for direction)
moveStep moveSequence[] = {
  { &M8, -stepsPerRevolution / 4, "M8" },    // M8: lower claws CCW
  { &M7, -stepsPerRevolution / 16, "M7" },   // M7: close claws CW
  { &M8,  stepsPerRevolution / 4, "M8" },    // M8: raise claws 
  { &M9,  stepsM9 / 2, "M9" },               // M9: rotate box CW
  { &M7,  stepsPerRevolution / 8, "M7" },    // M7: open claws
  { &M9, -stepsM9 / 2, "M9" }                // M9: rotate back CCW
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

  //setup serial monitor
  Serial.begin(baud);
  Serial.println("Pot release starting...");
  
  //setup M7, M8 & M9
  setupStepper(M7);
  setupStepper(M8);
  setupStepper(M9);
  
}


void loop() {
  
  //current step
  AccelStepper* motor = moveSequence[stepIndex].motor;
  int steps = moveSequence[stepIndex].steps;

  //begin move if not yet started
  if (!stepStarted) {
    motor->moveTo(motor->currentPosition() + steps);
    stepStarted = true;
    Serial.print("Step "); Serial.print(stepIndex);
    Serial.print(": Motor "); Serial.print(moveSequence[stepIndex].name);
    Serial.print(" → Steps "); Serial.println(steps);
  }

  //run motor
  motor->run();

  //is move complete
  if (stepStarted && motor->distanceToGo() == 0) {
    stepStarted = false;
    stepIndex++;
    lastStepTime = millis();
    delay(dt);  // Optional pause between steps
  }

  //end of sequence
  if (stepIndex >= numSteps) {
    Serial.println("Pot release sequence complete.");
    return;
  }

}
