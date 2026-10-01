# IR Beacon Delivery

**Language:** ROBOTC (C for the VEX Cortex)

**Build environment:** Windows with the ROBOTC IDE for the VEX Cortex. The program is downloaded from ROBOTC onto a VEX clawbot. There is no command-line build, and the code does not run on a desktop.

Drive a clawbot through a delivery routine. After a button press it backs away from a nearby wall, spins until an infrared sensor sees a beacon, approaches using the sonar and IR sensors, drops a payload, and parks.

## Breakdown

The robot moves through the states `standby`, `clearance`, `detection`, `approach`, `deliver`, and `park`. Motors are a left and right drive pair plus a payload motor. Sensors are two IR reflectance inputs, a touch button, a sonar, and quadrature encoders on the drive motors.

- `Final Program.c` — the full delivery state machine. This is the program to download.
- `Payload.c` — earlier payload-motor experiment
- `Move&Turn.c` — drive and point-turn helpers
- `IR-sensor.c` — reading the infrared beacon
- `Forward for Target Distance Clawbot.c` — drive a set distance
- `Forward for Distance with PID Clawbot.c` — the same drive using PID on the encoders
- `SourceFile002.c` — an early scratch file

## Usage

1. Open ROBOTC and load `Final Program.c`.
2. Confirm the motor and sensor ports match the clawbot (the `#pragma config` block at the top is the wiring).
3. Download the program to the Cortex and run it.
4. Press the touch button to leave standby. The robot then clears the wall, searches for the beacon, delivers, and parks.

The smaller files are earlier labs. Open one of those instead when you only want driving, turning, or the IR sensor.
