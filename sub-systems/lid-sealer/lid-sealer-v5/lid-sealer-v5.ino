// lid sealer v5 – diagnostic + sequential movement for M5 & M6

#include <AccelStepper.h>

// M5: accelStepper object
AccelStepper M6(AccelStepper::FULL4WIRE, 8, 10, 9, 11);

// M6: accelStepper object
AccelStepper M5(AccelStepper::FULL4WIRE, 3, 5, 4, 6);

const int stepsPerRevolution = 2048;
const int stepsM5 = stepsPerRevolution * 9; //M5 is attached to gear ratio of 9:1
const float rpm      = 5.0; 
const float motSpeed = rpm * stepsPerRevolution / 60.0; // steps/sec
const float motAccel = 100.0;                           // steps/sec²
const int dt         = 1000;                            // ms between cycles

// Your intended move sequence (signed for direction)
int moveSequence[][2] = {
  { -stepsM5/4,   0 },   // M5 up half  (CCW)
  {  0,   stepsPerRevolution/4 },   // M6 up half  (CW)
  {  stepsM5/4,   0 },   // M5 down half(CW)
  {  0,   stepsPerRevolution/4 },   // M6 up half  (CW)
  { -stepsM5/4,   0 },   // M5 up half  (CCW)
  {  0,  -stepsPerRevolution/2 },   // M6 down full(CCW)
  { -stepsM5/4,   0 },   // M5 up half  (CCW)
  {  stepsM5/2,   0 }    // M5 down full(CW)
};

int stepIndex = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("starting diagnostics");

  // Configure M5 & M6
  M5.setMaxSpeed(motSpeed);
  M5.setAcceleration(motAccel);
  M6.setMaxSpeed(motSpeed);
  M6.setAcceleration(motAccel);
}

// helper: move one motor synchronously
void moveMotor(AccelStepper & motor, int steps) {
  motor.move(steps);
  while (motor.distanceToGo() != 0) {
    motor.run();
  }
}

void loop() {
  // once both finished and delay elapsed, load next step
  if (!M5.isRunning() && !M6.isRunning() && millis() - lastTime >= dt) {
    // debug print
    Serial.print("Step "); Serial.print(stepIndex);
    Serial.print(" | M5 → "); Serial.print(moveSequence[stepIndex][0]);
    Serial.print(" | M6 → "); Serial.println(moveSequence[stepIndex][1]);

    // execute moves
    moveMotor(M5, moveSequence[stepIndex][0]);
    moveMotor(M6, moveSequence[stepIndex][1]);

    // advance index
    if (++stepIndex >= sizeof(moveSequence)/sizeof(moveSequence[0])) {
      stepIndex = 0;
      Serial.println("Sequence looped");
    }
    lastTime = millis();
  }
}
