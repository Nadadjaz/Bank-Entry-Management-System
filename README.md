# Bank Entry & Monitoring System 🏦

A combined security system that uses a single button to switch between four operational modes, controlling LEDs and a servo motor. Built by merging two earlier projects: the LED mode controller and the servo motor scanner (Bank Monitoring).

---

## How it works

One button cycles through 4 security states:

| Mode | State | Green LED | Red LED | Servo |
|------|-------|-----------|---------|-------|
| 0 | IDLE | OFF | OFF | Parked at 0° |
| 1 | ENTRY OPEN | ON | OFF | Sweeps to 90° (door opens) |
| 2 | MONITORING | OFF | ON | Scans back and forth 0°↔90° |
| 3 | ALERT | Blinking | Blinking | Locked at 90° |

The servo in Mode 2 scans continuously using non-blocking timing (`millis()`), so the button stays responsive at all times.

---

## Wiring

| Component | Pin |
|-----------|-----|
| Button | 2 (INPUT_PULLUP) |
| Green LED | 4 |
| Red LED | 5 |
| Servo signal | 9 |

---

## What I learned building this

Combining two separate projects into one wasn't just copy-paste. The main challenge was making the servo sweep non-blocking — the original bank monitoring code used `delay()` which would freeze the button. I replaced it with `millis()`-based timing so all modes stay responsive.

---

## Built with

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=Arduino&logoColor=white)

**Key concepts:** State machine, non-blocking timing with `millis()`, debounce, servo control, `Servo.h`

---

## Author

**Nada Djazari** — Electronics of Embedded Systems Engineer
📧 nada.djazari@gmail.com | 🎓 University of Jijel, 
