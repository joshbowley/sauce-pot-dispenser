//single pot - full code v1

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

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
