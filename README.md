# Sauce Pot Dispenser – Arduino Control System

This repository contains the full Arduino control code for a multi-stepper motor sauce pot dispensing system, developed as part of the Arthur Clements Engineering Scholarship (Summer 2025).

The machine was designed for Knowle Fish and Chip Shop to automate the preparation of free sauce pots, replacing a previously manual process. Automation reduces staff workload during busy periods, improves consistency, and increases throughput.

## System Overview

The system coordinates ten stepper motors using an Arduino Mega and the AccelStepper library. Each motor performs a specific mechanical function within the overall workflow:

| Motor | Function |
|-------|----------|
| M1    | Cup dispenser |
| M2    | Conveyor belt (three stages) |
| M3–M4 | Sauce dispenser (fill and retract) |
| M5    | Lid sealer – vertical press |
| M6    | Lid sealer – rotation |
| M7    | Pot release claws – open/close |
| M8    | Pot release claws – lift/lower |
| M9    | Release box rotation |
| M10   | Pot release claws – synchronised movement |

The control architecture is built around a non-blocking state machine, allowing multiple motors to be updated simultaneously without delays.

## Key Features

- Automated multi-stage dispensing, filling, sealing and release  
- Arduino Mega controlling ten stepper motors  
- AccelStepper for precise, non-blocking motor movement  
- Structured state machine for predictable, repeatable operation  
- Developed to meet real operational requirements in a commercial environment  

## Machine Sequence

1. CUP_DISPENSE – dispense an empty pot  
2. CONVEYOR1 – move pot to filling station  
3. SAUCE_DISPENSE – fill pot with sauce  
4. CONVEYOR2 – move to lid sealing station  
5. LID_SEAL – apply lid using press and rotation  
6. CONVEYOR3 – move to release station  
7. POT_RELEASE – lift, rotate and drop the pot  
8. DONE – wait before restarting the cycle  

All stages run without blocking calls, allowing smooth, continuous motor control.

## Requirements

- Arduino Mega 2560  
- AccelStepper library  
- External motor power supply  
- Ten 28BYJ-48 stepper motors (ULN2003 or A4988 drivers depending on configuration)  

## File Contents

- sauce_dispenser_v3.ino – full Arduino control program  
- Configuration parameters and motor assignments included within the code  

## Media

Prototype photos, design sketches and demonstration videos will be added in a future update.

## Developed By

Josh Bowley  
Physics Undergraduate, University of Exeter  
Arthur Clements Engineering Scholar 2025

Collaborator: Josh Rafnson-Hall

## Acknowledgements

- Knowle Fish and Chip Shop  
- Arthur Clements Engineering Scholarship, University of Bath  
- University of Exeter


## 🧵 Related Hashtags

#Arduino #StepperMotors #Automation #Engineering #EmbeddedSystems #CPlusPlus #MakerProject #FoodTech #MechanicalEngineering

> #Arduino #EmbeddedSystems #MotorControl #Automation #Cplusplus #AccelStepper #Mechatronics #EngineeringProject #UniversityOfExeter #UniversityOfBath #KnowleFishAndChips
