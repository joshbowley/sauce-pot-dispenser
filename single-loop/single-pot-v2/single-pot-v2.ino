//single pot - full code v2

#include <AccelStepper.h>

//=====================================================
//PIN ALLOCATION:

//pot release: ---------------------------------------

//m1 pins
#define M1_IN1  6
#define M1_IN2  7
#define M1_IN3  8
#define M1_IN4  9

//conveyor belt: --------------------------------------

//m2 pins
#define M2_IN1  37
#define M2_IN2  35
#define M2_IN3  33
#define M2_IN4  31

//sauce dispenser: ------------------------------------

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

//lid sealer: ------------------------------------------

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

//pot-release: -----------------------------------------

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

//======================================================
//CREATE STEPPER OBJECTS: 

AccelStepper M1(AccelStepper::FULL4WIRE, M1_IN1, M1_IN3, M1_IN2, M1_IN4);
AccelStepper M2(AccelStepper::FULL4WIRE, M2_IN1, M2_IN3, M2_IN2, M2_IN4);
AccelStepper M3(AccelStepper::FULL4WIRE, M3_IN1, M3_IN3, M3_IN2, M3_IN4);
AccelStepper M4(AccelStepper::FULL4WIRE, M4_IN1, M4_IN3, M4_IN2, M4_IN4);
AccelStepper M5(AccelStepper::FULL4WIRE, M5_IN1, M5_IN3, M5_IN2, M5_IN4);
AccelStepper M6(AccelStepper::FULL4WIRE, M6_IN1, M6_IN3, M6_IN2, M6_IN4);
AccelStepper M7(AccelStepper::FULL4WIRE, M7_IN1, M7_IN3, M7_IN2, M7_IN4);
AccelStepper M8(AccelStepper::FULL4WIRE, M8_IN1, M8_IN3, M8_IN2, M8_IN4);
AccelStepper M9(AccelStepper::FULL4WIRE, M9_IN1, M9_IN3, M9_IN2, M9_IN4);

//=====================================================
//MOTOR SETTINGS:

//steps
const int stepsPerRevolution = 2048;
const int stepsM6 = stepsPerRevolution * 9;    // m6 gear 9:1
const int stepsM9 = stepsPerRevolution * 4;    // m9 gear 4:1

//movement settings
const float rpm = 5.0;
const float motSpeed = rpm * stepsPerRevolution / 60.0;
const float motAccel = 100.0;

//wait time between steps
const int stepDelay = 1000;  // ms between steps

//=========================================================
//MACHINE STATE CONTROLLER:

//numbered states
enum StepState {
  IDLE,
  CUP_DISPENSE,
  CONVEYOR1,
  SAUCE_DISPENSE,
  CONVEYOR2,
  LID_SEAL,
  CONVEYOR3,
  POT_RELEASE,
  DONE
};

StepState currentState = CUP_DISPENSE;
bool stepStarted = false;
unsigned long lastStepTime = 0;

//=========================================================
//POT RELEASE: movement controller (m7, m8, m9)

//datastructure for movement information
struct moveStep {
  AccelStepper* motor;
  int steps;
  const char* name;
};

//movement sequence
moveStep potReleaseSequence[] = {
  { &M8, -stepsPerRevolution / 4, "M8" },    //lower claws
  { &M7, -stepsPerRevolution / 16, "M7" },   //close claws
  { &M8,  stepsPerRevolution / 4, "M8" },    //raise claws
  { &M9,  stepsM9 / 2, "M9" },               //rotate box cw
  { &M7,  stepsPerRevolution / 8, "M7" },    //open claws
  { &M9, -stepsM9 / 2, "M9" }                //rotate box ccw
};

const int numPotSteps = sizeof(potReleaseSequence) / sizeof(potReleaseSequence[0]);
int potStepIndex = 0;
bool potStepStarted = false;
unsigned long potLastStepTime = 0;

//run pot-release sequence
bool runPotRelease() {
  if (potStepIndex >= numPotSteps) {
    Serial.println("pot release complete");
    return true;
  }

  AccelStepper* motor = potReleaseSequence[potStepIndex].motor;
  int steps = potReleaseSequence[potStepIndex].steps;

  if (!potStepStarted) {
    motor->moveTo(motor->currentPosition() + steps);
    potStepStarted = true;
    Serial.print("step "); Serial.print(potStepIndex);
    Serial.print(": motor "); Serial.print(potReleaseSequence[potStepIndex].name);
    Serial.print(" → steps "); Serial.println(steps);
  }

  motor->run();

  if (potStepStarted && motor->distanceToGo() == 0) {
    potStepStarted = false;
    potStepIndex++;
    potLastStepTime = millis();
    delay(stepDelay);  // pause between steps
  }

  return false;
}

//=======================================
//SETUP: 

//motor setup function to simplify
void setupMotor(AccelStepper& motor) {
  motor.setMaxSpeed(motSpeed);
  motor.setAcceleration(motAccel);
  motor.setCurrentPosition(0);
}

//setup  
void setup() {
  Serial.begin(9600);
  Serial.println("Single Pot Filler Starting...");

  setupMotor(M1);
  setupMotor(M2);
  setupMotor(M3);
  setupMotor(M4);
  setupMotor(M5);
  setupMotor(M6);
  setupMotor(M7);
  setupMotor(M8);
  setupMotor(M9);
}

//=============================================================================================
//MAIN LOOP:

void loop() {
  switch (currentState) {

    case CUP_DISPENSE:
      if (!stepStarted) {
        M1.moveTo(M1.currentPosition() - stepsPerRevolution);
        stepStarted = true;
        Serial.println("CUP_DISPENSE");
      }
      M1.run();
      if (stepStarted && M1.distanceToGo() == 0) {
        stepStarted = false;
        currentState = CONVEYOR1;
        lastStepTime = millis();
      }
      break;

    case CONVEYOR1:
      if (!stepStarted) {
        M2.moveTo(M2.currentPosition() + stepsPerRevolution);
        stepStarted = true;
        Serial.println("CONVEYOR1");
      }
      M2.run();
      if (stepStarted && M2.distanceToGo() == 0) {
        stepStarted = false;
        currentState = SAUCE_DISPENSE;
        lastStepTime = millis();
      }
      break;

    case SAUCE_DISPENSE:
      if (!stepStarted) {
        M3.moveTo(M3.currentPosition() + stepsPerRevolution);
        M4.moveTo(M4.currentPosition() - stepsPerRevolution);
        stepStarted = true;
        Serial.println("SAUCE_DISPENSE");
      }
      M3.run();
      M4.run();
      if (stepStarted && M3.distanceToGo() == 0 && M4.distanceToGo() == 0) {
        stepStarted = false;
        currentState = CONVEYOR2;
        lastStepTime = millis();
      }
      break;

    case CONVEYOR2:
      if (!stepStarted) {
        M2.moveTo(M2.currentPosition() + stepsPerRevolution);
        stepStarted = true;
        Serial.println("CONVEYOR2");
      }
      M2.run();
      if (stepStarted && M2.distanceToGo() == 0) {
        stepStarted = false;
        currentState = LID_SEAL;
        lastStepTime = millis();
      }
      break;

    case LID_SEAL:
      static int lidStep = 0;
      static bool lidStepStarted = false;

      if (!lidStepStarted) {
        switch (lidStep) {
          case 0: M6.moveTo(M6.currentPosition() + stepsM6 / 4); break;
          case 1: M5.moveTo(M5.currentPosition() + stepsPerRevolution / 4); break;
          case 2: M6.moveTo(M6.currentPosition() - stepsM6 / 4); break;
          case 3: M5.moveTo(M5.currentPosition() + stepsPerRevolution / 4); break;
          case 4: M6.moveTo(M6.currentPosition() + stepsM6 / 4); break;
          case 5: M5.moveTo(M5.currentPosition() - stepsPerRevolution / 2); break;
          case 6: M6.moveTo(M6.currentPosition() + stepsM6 / 4); break;
          case 7: M6.moveTo(M6.currentPosition() - stepsM6 / 2); break;
        }
        lidStepStarted = true;
        Serial.print("LID_SEAL STEP "); Serial.println(lidStep);
      }

      M5.run();
      M6.run();

      bool M5Done = M5.distanceToGo() == 0;
      bool M6Done = M6.distanceToGo() == 0;
      

      if (lidStepStarted && M5Done && M6Done) {
        lidStep++;
        lidStepStarted = false;
        delay(stepDelay);
      }

      if (lidStep >= 8) {
        lidStep = 0;
        currentState = CONVEYOR3;
        stepStarted = false;
        lastStepTime = millis();
      }
      break;

    case CONVEYOR3:
      if (!stepStarted) {
        M2.moveTo(M2.currentPosition() + stepsPerRevolution);
        stepStarted = true;
        Serial.println("CONVEYOR3");
      }
      M2.run();
      if (stepStarted && M2.distanceToGo() == 0) {
        stepStarted = false;
        currentState = POT_RELEASE;
        lastStepTime = millis();
      }
      break;

    case POT_RELEASE:
      if (runPotRelease()) {
        potStepIndex = 0;
        potStepStarted = false;
        currentState = DONE;
        lastStepTime = millis();
      }
      break;

    case DONE:
      Serial.println("POT CYCLE COMPLETE");
      delay(5000);  // Optional delay before next cycle
      currentState = CUP_DISPENSE;
      break;

    default:
      break;
  }
}
