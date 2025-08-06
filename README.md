# 🥫 Sauce Pot Dispenser – Arduino Control System

This repository contains the full embedded control code for a **multi-stepper motor sauce pot dispenser**, developed as part of my **Arthur Clements Engineering Scholarship** project in summer 2025.

The system was designed to **automate the preparation of free sauce pots**, a key offering at **Knowle Fish and Chip Shop**, where they’re currently filled by hand. The shop operates with the same staffing levels regardless of whether pots are being filled, so automating this task can free up a staff member for front-of-house service — speeding up customer flow during peak hours without increasing cost.

---

## 🔧 System Overview

The full system coordinates **10 stepper motors** (M1–M10), each assigned to a different stage of the pot preparation and delivery process:

| Motor | Function                           |
|-------|------------------------------------|
| M1    | Cup Dispenser                      |
| M2    | Conveyor Belt (3 stages)           |
| M3–M4 | Sauce Dispenser (fill and retract) |
| M5–M6 | Lid Sealer (vertical + rotary)     |
| M7    | Pot Release Claws (open/close)     |
| M8    | Pot Release Claws (lift/lower)     |
| M9    | Release Box Rotation               |
| M10   | Pot Release (mirrors M8 for torque)|

---

## 🧠 Key Features

- **C++ on Arduino Mega**
- **AccelStepper** library for smooth, non-blocking motor control
- Modular **state machine architecture** to control the machine sequence
- Multi-motor coordination (especially M8 & M10 for torque-matched lifting)
- Serial debug output for live monitoring
- Custom motion sequences and timing control

---

## 🚦 Machine Sequence

1. **CUP_DISPENSE** – Dispenses pot
2. **CONVEYOR1** – Moves cup to filling station
3. **SAUCE_DISPENSE** – Dispenses sauce
4. **CONVEYOR2** – Moves to lid sealing station
5. **LID_SEAL** – Applies lid using vertical press + rotation
6. **CONVEYOR3** – Final conveyor movement
7. **POT_RELEASE** – Lifts, rotates, and drops pot using claw + box system
8. **DONE** – Waits before restarting cycle

Each stage is non-blocking and uses real-time motor updates through `motor.run()`.

---

## 🛠️ Requirements

- **Arduino Mega 2560** (recommended for pin count)
- **AccelStepper Library** (install via Arduino Library Manager)
- **External motor power supply** (for reliable torque)
- 10x 28BYJ-48 stepper motors with ULN2003 or A4988 drivers

---

## 📁 File Contents

- `sauce_dispenser_v3.ino` — Full control code (as seen in this 

## 📸 Media (Coming Soon)

- Prototype photos
- System demo video
- Design sketches and diagrams

---

## 👷‍♂️ Developed By

**Josh Bowley**  
Physics Undergraduate, University of Exeter  
Arthur Clements Engineering Scholar, Summer 2025  
Collaborator: [Josh Rafnson-Hall](https://www.linkedin.com/in/josh-rafnson-hall)

---

## 🏆 Acknowledgements

- **Knowle Fish and Chip Shop** — for inspiration, feedback, and real-world insights
- **University of Bath** — Arthur Clements Fund
- **University of Exeter** — for academic support and resources

---

## 🧵 Related Hashtags

> #Arduino #EmbeddedSystems #MotorControl #Automation #Cplusplus #AccelStepper #Mechatronics #EngineeringProject #UniversityOfExeter #UniversityOfBath #KnowleFishAndChips