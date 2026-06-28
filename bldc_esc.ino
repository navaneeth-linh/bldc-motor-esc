// ============================================================
// Sensorless BLDC Motor ESC — Arduino Nano
// ------------------------------------------------------------
// Motor    : A2212/13T 1000KV BLDC
// MOSFETs  : 6x IRF3205 (3-phase H-bridge)
// Drivers  : 3x IR2110 half-bridge gate drivers
// Control  : Potentiometer (A6) for variable speed
// Method   : Open-loop six-step commutation with startup ramp
// Tested   : 12V nominal, up to 18V
// Author   : S Navaneeth Krishna
// ============================================================

// --- Pin Definitions ---
// AH/AL = Phase A High/Low side MOSFET gate signals
// BH/BL = Phase B High/Low side MOSFET gate signals
// CH/CL = Phase C High/Low side MOSFET gate signals
int AH = 2, AL = 3;
int BH = 4, BL = 5;
int CH = 6, CL = 7;

// Potentiometer connected to analog pin A6 for speed control
int potPin = A6;

// Flag to track if startup ramp has completed
bool started = false;

void setup() {
  // Set all MOSFET gate pins as outputs
  pinMode(AH, OUTPUT); pinMode(AL, OUTPUT);
  pinMode(BH, OUTPUT); pinMode(BL, OUTPUT);
  pinMode(CH, OUTPUT); pinMode(CL, OUTPUT);

  // Ensure all MOSFETs are OFF at startup — prevents shoot-through
  allOff();
}

void loop() {
  // Read potentiometer value (0–1023)
  int potValue = analogRead(potPin);

  // Map pot value to commutation delay in microseconds
  // Higher delay = slower speed, Lower delay = faster speed
  int targetDelay = map(potValue, 0, 1023, 3000, 300);

  // Safety threshold — stop motor if pot is below minimum
  // Prevents accidental startup at near-zero throttle
  if (potValue < 50) {
    started = false;
    allOff();
    return;
  }

  // Startup ramp — runs only once when motor is first started
  // Gradually decreases commutation delay from 3000us to target
  // This allows rotor to build momentum before running at full speed
  if (!started) {
    for (int i = 3000; i > targetDelay; i -= 30) {
      commutate(i);
    }
    started = true;
  }

  // Continuous commutation — runs all 6 steps at target speed
  commutate(targetDelay);
}

// ------------------------------------------------------------
// commutate() — Executes one full electrical cycle (6 steps)
// d = delay in microseconds between each commutation step
// ------------------------------------------------------------
void commutate(int d) {
  step1(); delayMicroseconds(d); deadTime();
  step2(); delayMicroseconds(d); deadTime();
  step3(); delayMicroseconds(d); deadTime();
  step4(); delayMicroseconds(d); deadTime();
  step5(); delayMicroseconds(d); deadTime();
  step6(); delayMicroseconds(d); deadTime();
}

// ------------------------------------------------------------
// deadTime() — Brief OFF period between commutation steps
// Prevents shoot-through (simultaneous high+low MOSFET ON)
// 10 microseconds is sufficient for IRF3205 turn-off time
// ------------------------------------------------------------
void deadTime() {
  allOff();
  delayMicroseconds(10);
}

// ------------------------------------------------------------
// allOff() — Turns off all 6 MOSFETs simultaneously
// Called at the start of every step and during dead time
// ------------------------------------------------------------
void allOff() {
  digitalWrite(AH, LOW); digitalWrite(AL, LOW);
  digitalWrite(BH, LOW); digitalWrite(BL, LOW);
  digitalWrite(CH, LOW); digitalWrite(CL, LOW);
}

// ------------------------------------------------------------
// Six-Step Commutation Sequence
// Each step energizes two phases — one high, one low
// Third phase remains floating for back-EMF sensing
// allOff() called first in every step to prevent shoot-through
// ------------------------------------------------------------

// Step 1: Phase A+ to Phase B-
void step1() { allOff(); digitalWrite(AH, HIGH); digitalWrite(BL, HIGH); }

// Step 2: Phase A+ to Phase C-
void step2() { allOff(); digitalWrite(AH, HIGH); digitalWrite(CL, HIGH); }

// Step 3: Phase B+ to Phase C-
void step3() { allOff(); digitalWrite(BH, HIGH); digitalWrite(CL, HIGH); }

// Step 4: Phase B+ to Phase A-
void step4() { allOff(); digitalWrite(BH, HIGH); digitalWrite(AL, HIGH); }

// Step 5: Phase C+ to Phase A-
void step5() { allOff(); digitalWrite(CH, HIGH); digitalWrite(AL, HIGH); }

// Step 6: Phase C+ to Phase B-
void step6() { allOff(); digitalWrite(CH, HIGH); digitalWrite(BL, HIGH); }
