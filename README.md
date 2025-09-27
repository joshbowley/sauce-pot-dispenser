# 🥫 Sauce Pot Dispenser – Arduino Control System

This repository contains the **full Arduino control code** for a **multi-stepper motor sauce pot dispenser**, developed as part of the **Arthur Clements Engineering Scholarship** project in Summer 2025.

The system was designed to **automate the preparation of free sauce pots** at **Knowle Fish and Chip Shop**, which were previously filled manually. Automating this process reduces staff workload during peak hours, improves customer service efficiency, and maintains consistent portion control.

---

## 🔧 System Overview

The machine coordinates **10 stepper motors** (M1–M10), each handling a specific stage of pot preparation and delivery:

| Motor | Function                           |
|-------|------------------------------------|
| M1    | Cup Dispenser                      |
| M2    | Conveyor Belt (3 stages)           |
| M3–M4 | Sauce Dispenser (fill & retract)  |
| M5    | Lid Sealer (vertical press)        |
| M6    | Lid Sealer (rotation)              |
| M7    | Pot Release Claws (open/close)     |
| M8    | Pot Release Claws (lift/lower)     |
| M9    | Release Box Rotation               |
| M10   | Pot Release Claws (sync with M8)   |

The system uses **C++ on an Arduino Mega** with the **AccelStepper library** for precise, non-blocking motor control.

---

## Key Features
- **Automated Filling:** Dispenses sauce into pots using a conveyor system and multiple stepper motors.  
- **Multi-Motor Control:** Arduino Mega coordinates 10 stepper motors for conveyor, cup dispensing, sauce dispensing, lid sealing, and pot release.  
- **Scalable Architecture:** Non-blocking state machine for synchronised motor operation.  
- **Client-Focused Design:** Developed based on operational requirements of a real business.  
---

## 🚦 Machine Sequence

1. **CUP_DISPENSE** – Dispense a pot  
2. **CONVEYOR1** – Move pot to filling station  
3. **SAUCE_DISPENSE** – Fill pot with sauce  
4. **CONVEYOR2** – Move to lid sealing station  
5. **LID_SEAL** – Apply lid using vertical press + rotation  
6. **CONVEYOR3** – Move to pot release station  
7. **POT_RELEASE** – Lift, rotate, and drop pot using claws & box  
8. **DONE** – Wait before restarting cycle  

Each stage is **non-blocking**, allowing smooth simultaneous motor updates.

---

## 🛠️ Requirements

- **Arduino Mega 2560** (high pin count for multiple motors)  
- **AccelStepper Library** (install via Arduino Library Manager)  
- **External motor power supply** for stable torque  
- 10× 28BYJ-48 stepper motors (ULN2003 or A4988 drivers)  

---

## 📁 File Contents

- `sauce_dispenser_v3.ino` — Full Arduino code for single-pot operation  
- Configuration and motor setup embedded in code  

---

## 📸 Media (Coming Soon)

- Prototype photos  
- Demo video  
- Design sketches and diagrams  

---

## 👷‍♂️ Developed By

**Josh Bowley**  
Physics Undergraduate, University of Exeter  
Arthur Clements Engineering Scholar, Summer 2025  

Collaborator: [Josh Rafnson-Hall](https://www.linkedin.com/in/joshua-rafnson-hall-974ba2323?utm_source=share&utm_campaign=share_via&utm_content=profile&utm_medium=ios_app)  

---

## 🏆 Acknowledgements

- **Knowle Fish and Chip Shop** — inspiration, feedback, and real-world insights  
- **University of Bath** — Arthur Clements Fund  
- **University of Exeter** — academic support and resources  

---

## 🧵 Related Hashtags

#Arduino #StepperMotors #Automation #Engineering #EmbeddedSystems #CPlusPlus #MakerProject #FoodTech #MechanicalEngineering

> #Arduino #EmbeddedSystems #MotorControl #Automation #Cplusplus #AccelStepper #Mechatronics #EngineeringProject #UniversityOfExeter #UniversityOfBath #KnowleFishAndChips
