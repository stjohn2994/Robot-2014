#ifndef BASICDEFINES_H_
#define BASICDEFINES_H_

// Define which robot we are compiling for (ROBOT_A, ROBOT_B, or PLYBOY)
// PLYBOY - Test platform.
#define PLYBOY

// ROBOT_A - Competition Robot.
//#define ROBOT_A

// ROBOT_B - Practice Robot.
//#define ROBOT_B

// PWMs
#define LEFT_FRONT_DRIVE_PWM 1
#define LEFT_REAR_DRIVE_PWM 2
#define RIGHT_FRONT_DRIVE_PWM 3
#define RIGHT_REAR_DRIVE_PWM 4
#define INTAKE_MOTOR_PWM 5
#define WINCH_MOTOR_PWM 6

// Solenoids
#define SHIFTER_A 1
#define SHIFTER_B 2
#define ARM_A 3
#define ARM_B 4
#define EJECT_A 5
#define EJECT_B 6

// Digital I/O
#define LEFT_ENCODER_A 2
#define LEFT_ENCODER_B 3
#define RIGHT_ENCODER_A 4
#define RIGHT_ENCODER_B 5
#define COMPRESSOR_PRESSURE_SW 8
#define WINCH_SWITCH 11

// Relays
#define COMPRESSOR_SPIKE 2

// USB ports
#define RIGHT_DRIVE_STICK 1
#define LEFT_DRIVE_STICK 2
#define GAMEPAD_PORT 3

// Left Joystick
#define BUTTON_SHIFT 7

// Gamepad
#define BUTTON_SHOOT 1
#define BUTTON_LOAD 4
#define BUTTON_ARM 5
#define BUTTON_PASS 7

// Motor speeds
#define WINCH_FWD 1.0

// Misc.
#define LOOP_PERIOD 0.01
// # of encoder ticks to drive for in auto
// TODO: Determine empirically.
#define ENCODER_DIST 9000
// Time to wait after running motor to launch shooter before turning motor off
#define CATAPULT_SHOOT_WAIT 0.5
// Wait before turning off the eject pistons to ensure the ball is out.
#define EJECT_WAIT 2.0
// Speed to drive at in autonomous
#define AUTO_DRIVE_SPEED 0.5

#ifdef PLYBOY
#define ROBOT_NAME "PLYBOY"
#define LEFT_DRIVE_MOTOR Jaguar
#define RIGHT_DRIVE_MOTOR Jaguar
#define INTAKE_MOTOR Jaguar
#define WINCH_MOTOR Jaguar
#endif // PLYBOY

#ifdef ROBOT_A
#define ROBOT_NAME "Comp. Bot"
#define LEFT_DRIVE_MOTOR Talon
#define RIGHT_DRIVE_MOTOR Talon
#define INTAKE_MOTOR Talon
#define WINCH_MOTOR Talon
#endif // ROBOT_A

#ifdef ROBOT_B
#define ROBOT_NAME "Practice Bot"
#define LEFT_DRIVE_MOTOR Victor
#define RIGHT_DRIVE_MOTOR Victor
#define INTAKE_MOTOR Victor
#define WINCH_MOTOR Victor
#endif // ROBOT_B

#endif // BASICDEFINES_H_
