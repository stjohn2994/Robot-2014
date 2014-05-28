#ifndef BASICDEFINES_H_
#define BASICDEFINES_H_

#include <WPILib.h>

#include "EGamepad.h"

// Define which robot we are compiling for (ROBOT_A, ROBOT_B, or PLYBOY)
// PLYBOY - Test platform.
//#define PLYBOY

// ROBOT_A - Competition Robot.
#define ROBOT_A

// ROBOT_B - Practice Robot.
//#define ROBOT_B

// PWMs
#define LEFT_FRONT_DRIVE_PWM 4   // cable 0 / 28
#define LEFT_REAR_DRIVE_PWM 3    // cable 44 / 26
#define RIGHT_FRONT_DRIVE_PWM 2  // cable 43 / 22
#define RIGHT_REAR_DRIVE_PWM 1   // cable 41 / 23
#define CENTER_LEFT_DRIVE_PWM 7  // UNKNOWN CABLE
#define CENTER_RIGHT_DRIVE_PWM 8 // UNKNOWN CABLE
#define INTAKE_MOTOR_PWM 5       // cable 34 / 21
#define RIGHT_WINCH_MOTOR_PWM 6        // cable 40 / 27
#define LEFT_WINCH_MOTOR_PWM 9

// Solenoids
#define SHIFTER_A 1 // cable 0
#define SHIFTER_B 2 // cable 00
#define ARM_A 3     // cable 22
#define ARM_B 4     // cable 23
#define EJECT_A 5   // cable 7
#define EJECT_B 6   // cable 12

// Digital I/O
#define LEFT_ENCODER_A 2         // cable 11
#define LEFT_ENCODER_B 3         // cable 12
#define RIGHT_ENCODER_A 4        // cable 39
#define RIGHT_ENCODER_B 5        // cable 40
#define COMPRESSOR_PRESSURE_SW 8 //
#define WINCH_SWITCH 11          //

// Relays
#define COMPRESSOR_SPIKE 2 // cable 7

// USB ports
#define RIGHT_DRIVE_STICK 1
#define LEFT_DRIVE_STICK 2
#define GAMEPAD_PORT 3

// Left Joystick
#define BUTTON_SHIFT 7
#define BUTTON_REVERSE 6

// Gamepad
#define BUTTON_SHOOT 1
#define BUTTON_INTAKE_COLLECT EGamepad::kUp
#define BUTTON_INTAKE_EJECT EGamepad::kDown
// LOAD THE WINCH
#define BUTTON_LOAD 4
#define BUTTON_ARM 5
#define BUTTON_PASS 7

// Motor speeds
#define WINCH_FWD 1.0
// Intake speeds
#define INTAKE_COLLECT -0.95
#define INTAKE_EJECT 0.95

// Misc.
#define LOOP_PERIOD 0.01
// # of encoder ticks to drive for in auto
// TODO: Determine empirically.
#define INTO_ZONE_DIST 2385
#define SHOT_POSN_DIST 1431
// Backwards drive for drivers
#define ENCODER_BACK_DIST 5500
// Time to wait after running motor to launch shooter before turning motor off
#define CATAPULT_SHOOT_WAIT 0.1
// Wait before turning off the eject pistons to ensure the ball is out.
#define EJECT_WAIT 2.0
// Speed to drive at in autonomous
#define AUTO_DRIVE_SPEED 0.75
// Amount of time to wait until we shoot after collecting second ball
#define AUTO_PICKUP_WAIT 2.0
// Amount of time to wait until we move after shooting second ball.
#define AUTO_SHOOT_WAIT 0.25

#ifdef PLYBOY
#define ROBOT_NAME "PLYBOY"
#define LEFT_DRIVE_MOTOR Jaguar
#define RIGHT_DRIVE_MOTOR Jaguar
#define CENTER_DRIVE_MOTOR Jaguar
#define INTAKE_MOTOR Jaguar
#define WINCH_MOTOR Jaguar
#endif // PLYBOY

#ifdef ROBOT_A
#define ROBOT_NAME "Comp. Bot"
#define LEFT_DRIVE_MOTOR Talon
#define RIGHT_DRIVE_MOTOR Talon
#define CENTER_DRIVE_MOTOR Talon
#define INTAKE_MOTOR Talon
#define WINCH_MOTOR Talon
#endif // ROBOT_A

#ifdef ROBOT_B
#define ROBOT_NAME "Practice Bot"
#define LEFT_DRIVE_MOTOR Victor
#define RIGHT_DRIVE_MOTOR Victor
#define CENTER_DRIVE_MOTOR Victor
#define INTAKE_MOTOR Victor
#define WINCH_MOTOR Victor
#endif // ROBOT_B

#endif // BASICDEFINES_H_
