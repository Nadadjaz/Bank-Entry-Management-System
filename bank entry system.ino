// ============================================
// Bank Entry & Monitoring System
// Combines: LED Mode Controller (Licence 3)
//         + Servo Motor Scanner (Bank Monitoring)
// Author: Nada Djazari — University of Jijel
// ============================================
//
// HOW IT WORKS:
//   A button cycles through 3 security modes:
//
//   Mode 0 — IDLE:
//     Both LEDs OFF, servo stopped at 0°
//
//   Mode 1 — ENTRY OPEN:
//     Green LED ON (access granted)
//     Servo sweeps 0°→90° (door/barrier opens)
//
//   Mode 2 — MONITORING:
//     Red LED ON (monitoring active)
//     Servo scans continuously 0°↔90° (camera scanning)
//
//   Mode 3 — ALERT:
//     Both LEDs blink rapidly (alarm)
//     Servo moves to 90° and stays (locked position)
//
// WIRING:
//   Button      → Pin 2  (INPUT_PULLUP)
//   Green LED   → Pin 4
//   Red LED     → Pin 5
//   Servo signal→ Pin 9
// ============================================

#include <Servo.h>

// ── Pin definitions ──
const int switchPin  = 2;  // Button
const int greenLed   = 4;  // Green LED (access granted)
const int redLed     = 5;  // Red LED (monitoring/alert)
const int SERVO_PIN  = 9;  // Servo motor

// ── Servo ──
Servo moteur;

// ── Button debounce variables ──
int val;
int val2;
int buttonState;

// ── System mode ──
int Mode = 0; // 0=IDLE, 1=ENTRY, 2=MONITORING, 3=ALERT

// ── Servo state for non-blocking sweep ──
int servoAngle    = 0;
int servoDir      = 1;       // 1 = going up, -1 = going down
unsigned long lastServoMove  = 0;
const int SERVO_STEP_DELAY   = 30; // ms between each degree

// ── Alert blink state ──
unsigned long lastBlink = 0;
bool blinkState = false;

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(greenLed,  OUTPUT);
  pinMode(redLed,    OUTPUT);

  moteur.attach(SERVO_PIN);
  moteur.write(0); // Start at 0°

  // All off at start
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed,   LOW);

  buttonState = digitalRead(switchPin);

  Serial.begin(9600);
  Serial.println("Bank Entry System — Ready");
}

void loop() {
  // ── Read button with debounce ──
  val  = digitalRead(switchPin);
  delay(10);
  val2 = digitalRead(switchPin);

  if (val == val2) {
    if (val != buttonState) {
      if (val == LOW) { // Button pressed
        Mode = (Mode + 1) % 4; // Cycle 0→1→2→3→0
        Serial.print("Mode changed to: ");
        Serial.println(Mode);
        // Reset servo when mode changes
        servoAngle = 0;
        servoDir   = 1;
        moteur.write(0);
      }
    }
    buttonState = val;
  }

  // ── Execute current mode ──
  switch (Mode) {

    // ── MODE 0: IDLE ──
    case 0:
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed,   LOW);
      moteur.write(0); // Servo parked at 0°
      break;

    // ── MODE 1: ENTRY OPEN ──
    // Green LED ON, servo sweeps once to 90° then stays
    case 1:
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed,   LOW);
      // Move servo toward 90° smoothly (non-blocking)
      if (servoAngle < 90) {
        if (millis() - lastServoMove >= SERVO_STEP_DELAY) {
          servoAngle++;
          moteur.write(servoAngle);
          lastServoMove = millis();
        }
      }
      break;

    // ── MODE 2: MONITORING ──
    // Red LED ON, servo scans back and forth continuously
    case 2:
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed,   HIGH);
      // Non-blocking continuous sweep
      if (millis() - lastServoMove >= SERVO_STEP_DELAY) {
        servoAngle += servoDir;
        if (servoAngle >= 90) { servoDir = -1; } // Reached 90° → go back
        if (servoAngle <= 0)  { servoDir =  1; } // Reached 0°  → go forward
        moteur.write(servoAngle);
        lastServoMove = millis();
      }
      break;

    // ── MODE 3: ALERT ──
    // Both LEDs blink fast, servo locked at 90°
    case 3:
      moteur.write(90); // Lock at 90°
      // Non-blocking blink every 200ms
      if (millis() - lastBlink >= 200) {
        blinkState = !blinkState;
        digitalWrite(greenLed, blinkState);
        digitalWrite(redLed,   blinkState);
        lastBlink = millis();
      }
      break;
  }
}
