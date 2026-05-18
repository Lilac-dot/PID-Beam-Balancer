<div align="center">

# PID Ball Balancer

### Real-Time Ball & Beam Stabilization using Arduino and PID Control

![Platform](https://img.shields.io/badge/Platform-Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Control](https://img.shields.io/badge/Control-PID-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)
![Domain](https://img.shields.io/badge/Domain-Control%20Systems-blueviolet?style=for-the-badge)

*A closed-loop control system that stabilizes a rolling ball on a beam using real-time PID feedback — built using Arduino, a servo motor, and an ultrasonic sensor.*

</div>

---

## Project Overview

The Ball and Beam system is a classic benchmark in control engineering — deceptively simple in appearance, yet fundamentally unstable by nature. A ball placed on a tilted beam will accelerate in the direction of the tilt with no mechanism to stop it. Even a tiny disturbance causes the ball to roll off unless continuously corrected.

This project implements a closed-loop PID (Proportional-Integral-Derivative) controller on an Arduino Uno to dynamically adjust the beam angle via a servo motor, keeping the ball stabilized at a desired setpoint.

**Why does this matter?** The same control principles govern:
- Aircraft pitch stabilization
- Robotic balancing systems
- Industrial precision positioning
- Rocket attitude control

---

## Features

- Real-time closed-loop feedback — continuous position sensing and correction
- PID-based stabilization — proportional, integral, and derivative actions combined
- Servo angle correction — smooth, responsive beam actuation
- Ultrasonic position sensing — non-contact ball detection along the beam
- Steady-state error elimination — via integral control action
- Adjustable PID tuning — easily configurable gain parameters in code

---

## Hardware Components

| Component | Model / Spec | Purpose |
|---|---|---|
| Microcontroller | Arduino Uno (ATmega328P) | Main processing unit & PID computation |
| Actuator | Servo Motor (SG90 / MG996R) | Controls beam tilt angle |
| Sensor | HC-SR04 Ultrasonic Sensor | Measures ball position along beam |
| Beam | Thermocol / Acrylic Rail | Guides ball movement |
| Ball | Lightweight Plastic Ball | Controlled object |
| Wiring | Jumper Wires | Signal and power connections |
| Power | 5V USB / External Supply | Powers Arduino and servo |

---

## Software & Technologies

| Tool | Purpose |
|---|---|
| Arduino IDE | Firmware development environment |
| Embedded C / C++ | Controller and sensor logic |
| PID Algorithm | Core control law implementation |
| MATLAB / Simulink *(optional)* | System modeling and simulation |

---

## System Architecture

```
┌─────────────────────────────┐
│      Ultrasonic Sensor      │  ← Measures ball position (cm)
└────────────┬────────────────┘
             │  position feedback
             ▼
┌─────────────────────────────┐
│   Arduino PID Controller    │  ← Computes error & PID output
│   error = setpoint - pos    │
│   u = Kp·e + Ki·∫e + Kd·ė  │
└────────────┬────────────────┘
             │  control signal (PWM)
             ▼
┌─────────────────────────────┐
│        Servo Motor          │  ← Converts signal to angle
└────────────┬────────────────┘
             │  mechanical actuation
             ▼
┌─────────────────────────────┐
│     Beam Angle Adjustment   │  ← Tilts beam CW / CCW
└────────────┬────────────────┘
             │
             ▼
┌─────────────────────────────┐
│   Ball Position Stabilized  │  ← Ball held at setpoint ✓
└─────────────────────────────┘
```

---

## Working Principle

1. **Sense** — The ultrasonic sensor fires a pulse and measures the time-of-flight to detect the ball's current position on the beam.
2. **Compute Error** — The Arduino calculates the difference between the desired setpoint and the measured position: `error = setpoint − position`
3. **Apply PID** — The controller computes a correction signal using all three PID terms.
4. **Actuate** — The correction signal is sent as a PWM command to the servo motor, adjusting the beam angle.
5. **Stabilize** — The beam tilt causes the ball to roll back toward the setpoint, closing the loop.

This cycle repeats continuously at the controller's sampling rate, maintaining stability in real time.

---

## PID Control Explained

The PID control law is:

```
u(t) = Kp · e(t)  +  Ki · ∫e(t)dt  +  Kd · de(t)/dt
```

### Proportional Control — `Kp`
Generates a correction **proportional to the current error**. A larger `Kp` produces faster response but risks overshoot and oscillation. Too small, and the ball drifts far from setpoint.

### Integral Control — `Ki`
Accumulates past error over time, **eliminating steady-state offset**. Without it, the ball may stabilize slightly off-target due to friction or gravity bias. Excessive `Ki` introduces slow oscillation.

### Derivative Control — `Kd`
Acts on the **rate of change of error**, damping the response before it overshoots. It smooths out servo movement and reduces oscillations, especially when the ball is approaching the setpoint quickly.

| Term | Affects | Too High | Too Low |
|---|---|---|---|
| `Kp` | Speed of response | Oscillation / overshoot | Sluggish response |
| `Ki` | Steady-state error | Slow oscillation, windup | Persistent offset |
| `Kd` | Damping | Noise amplification, jitter | Underdamped oscillation |

> **Tuned values used in this project:** `Kp = 4`, `Ki = 1`, `Kd = 2`

---

## Mathematical Modeling

### Ball Dynamics

The equation of motion for the ball along the beam, derived from Newton's second law:

```
m · ẍ = m · g · sin(θ)
```

Where:
- `m` — mass of the ball
- `ẍ` — linear acceleration of the ball along the beam
- `g` — acceleration due to gravity (9.81 m/s²)
- `θ` — beam tilt angle controlled by the servo

### Small-Angle Approximation

For small angles (valid for the operating range of the servo):

```
sin(θ) ≈ θ     →     m · ẍ = m · g · θ
```

This linearizes the nonlinear system, enabling classical PID design. The ball's acceleration is directly proportional to the beam angle — meaning **any nonzero angle causes the ball to accelerate**, confirming the system's inherent instability without feedback.

---

## Results & Observations

### Successful Outcomes

- Ball successfully **stabilized at the desired setpoint** with tuned PID gains
- **Steady-state error reduced to near zero** via integral action
- **Servo movement became visibly smoother** after derivative tuning
- **Overshoot reduced significantly** compared to proportional-only control
- Without any controller, the ball rolls off the beam — confirming the need for closed-loop control

### Observed Limitations

| Limitation | Cause | Impact |
|---|---|---|
| Servo jitter | Ultrasonic sensor noise | Minor oscillation near setpoint |
| Positional inaccuracy | Beam surface friction | Small steady-state deviation |
| Temporary instability | Large external disturbances | Ball briefly loses balance |

---

## Future Improvements

- LQR Controller — Implement Linear Quadratic Regulator for optimal state-space control
- Camera-Based Tracking — Replace ultrasonic sensor with OpenCV vision for higher accuracy
- Machine Learning Prediction — Use reinforcement learning to self-tune PID gains
- Wireless Monitoring — Add Bluetooth/Wi-Fi telemetry to log position data in real time
- Advanced Filtering — Apply Kalman filter or moving average to reduce sensor noise

---

## Setup Instructions

### 1. Hardware Connections

| Sensor / Component | Arduino Pin |
|---|---|
| Ultrasonic TRIG | Digital Pin 9 |
| Ultrasonic ECHO | Digital Pin 10 |
| Servo Signal | Digital Pin 6 |
| Servo VCC | 5V |
| All GND | GND |

### 2. Install Required Libraries

Open Arduino IDE → Sketch → Include Library → Manage Libraries, then install:

```
- Servo.h       (built-in with Arduino IDE)
- NewPing.h     (for ultrasonic sensor)
```

### 3. Upload the Code

```bash
# Clone the repository
git clone https://github.com/yourusername/PID-Ball-Balancer.git

# Open in Arduino IDE
File → Open → Arduino_Code/pid_ball_balancer.ino

# Select board and port
Tools → Board → Arduino Uno
Tools → Port → COMx (Windows) or /dev/ttyUSBx (Linux)

# Upload
Sketch → Upload
```

### 4. Servo Calibration

Before running the PID loop, calibrate the servo neutral position so the beam is perfectly horizontal:

```cpp
// In your code, adjust this value until the beam is level
servo.write(90);   // 90° = neutral / horizontal beam
```

### 5. PID Tuning Guide

```cpp
// Start with only Kp, set Ki and Kd to 0
float Kp = 2.0;
float Ki = 0.0;
float Kd = 0.0;

// Increase Kp until the ball oscillates, then back off slightly
// Add Kd to dampen oscillations
// Add small Ki to remove steady-state error
```

## Conclusion

This project successfully demonstrated closed-loop stabilization of an inherently unstable mechanical system using a PID controller on affordable embedded hardware. The experiment provided practical insight into system modeling, gain tuning, real-time control, and hardware-software integration. While sensor noise and friction presented minor challenges, the core control objective — keeping the ball at a desired position — was consistently achieved. The system serves as a strong foundation for exploring advanced control strategies such as LQR, model predictive control, or vision-based feedback.

---

## References

> [1] A. Taifour Ali, A. A. M., A. H. A., O. A. Taha, and A. Naseraldeen A., "Design and Implementation of Ball and Beam System Using PID Controller," *Automatic Control and Information Sciences*, vol. 3, no. 1, pp. 1–4, 2017.

---

⭐ *If you found this useful, consider starring the repo!*

</div>
