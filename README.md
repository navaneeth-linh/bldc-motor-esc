# 🔧 Sensorless BLDC Motor ESC — Custom Hardware

A custom-designed sensorless Electronic Speed Controller (ESC) for a BLDC motor, built on a custom PCB using Arduino Nano, IRF3205 MOSFETs, and IR2110 gate drivers. Implements six-step commutation with back-EMF zero-crossing detection for sensorless operation.

---

## 📌 Project Overview

| Parameter | Specification |
|---|---|
| **Motor** | A2212/13T 1000KV BLDC Motor |
| **Operating Voltage** | 12V nominal — tested up to 18V |
| **MOSFETs** | 6x IRF3205 (3 high-side, 3 low-side) |
| **Gate Drivers** | 3x IR2110 half-bridge drivers |
| **Microcontroller** | Arduino Nano |
| **Control** | Potentiometer — variable speed control |
| **Sensing** | Sensorless — back-EMF zero-crossing detection |
| **PCB** | Custom designed and fabricated |

---

## ⚙️ System Architecture

### Power Stage
```
12V Supply
    → 6x IRF3205 MOSFETs (3-phase H-bridge)
        Phase A → Motor Terminal A
        Phase B → Motor Terminal B
        Phase C → Motor Terminal C
```

### Control Stage
```
Arduino Nano
    → Generates 6-step PWM commutation signals
        → 3x IR2110 Gate Drivers
            → High-side and low-side MOSFET switching
                → BLDC Motor phases energized in sequence
```

### Speed Control
```
Potentiometer
    → Analog input to Arduino Nano
        → PWM duty cycle adjusted
            → Motor speed varies accordingly
```

### Feedback System
```
Motor back-EMF monitored on floating phase
    → Zero-crossing detected by Arduino
        → Commutation timing adjusted
            → Smooth sensorless operation maintained
```

---

## 🔌 Hardware Components

| Component | Specification | Quantity |
|---|---|---|
| Arduino Nano | ATmega328P microcontroller | 1 |
| IRF3205 MOSFET | 55V, 110A N-channel | 6 |
| IR2110 Gate Driver | High and low side driver | 3 |
| A2212/13T Motor | 1000KV BLDC motor | 1 |
| Potentiometer | Speed control input | 1 |
| Custom PCB | Designed and fabricated | 1 |

---

## 🧠 How It Works

### Six-Step Commutation
A 3-phase BLDC motor requires 6 commutation steps per electrical cycle. The Arduino Nano generates the correct switching sequence for all 6 MOSFETs to energize the motor phases in order.

### Sensorless Operation
Instead of Hall effect sensors, this ESC detects the back-EMF voltage on the floating (unenergized) phase. When the back-EMF crosses zero, the Arduino uses this signal to time the next commutation step — no physical sensors needed.

### Gate Driver Stage
The IR2110 is a half-bridge gate driver that handles both high-side and low-side MOSFET switching. Three IR2110 drivers control the 6 IRF3205 MOSFETs — one driver per motor phase.

### Speed Control
A potentiometer connected to the Arduino's analog input varies the PWM duty cycle sent to the gate drivers, which controls the average voltage applied to the motor and therefore its speed.

---

## 📁 Repository Structure

```
bldc-motor-esc/
├── media/
│   ├── pcb_photo.jpg
│   ├── schematic.pdf
│   └── test_video_link.md
├── ESC_code/
│   └── bldc_esc.ino
├── KiCad/
│   ├── schematic.kicad_sch
│   └── pcb_layout.kicad_pcb
├── README.md
```

---

## 🛠️ Tools Used

- **Arduino IDE** — Firmware development
- **KiCad** — Schematic and PCB design
- **Custom PCB** — Designed and fabricated in-house

---

## ⚠️ Key Challenges Solved

- Implementing reliable zero-crossing detection without dedicated comparator ICs
- Bootstrap capacitor timing for IR2110 high-side driver
- Dead-time implementation to prevent shoot-through in the H-bridge
- Stable commutation at varying load conditions

---

## 👤 Author

**S Navaneeth Krishna**
B.Tech — Electrical and Electronics Engineering
Muthoot Institute of Science & Technology, Kerala

[![GitHub](https://img.shields.io/badge/GitHub-navaneeth--linh-181717?style=flat&logo=github)](https://github.com/navaneeth-linh)

---

## 📜 License

This project is open source and available for educational reference.
