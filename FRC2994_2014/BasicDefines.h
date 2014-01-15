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
#define INTAKE_MOTOR_1_PWM 5
#define INTAKE_MOTOR_2_PWM 6
#define WINCH_MOTOR_PWM 7

// Solenoids
#define SHIFTER_A 1
#define SHIFTER_B 2
#define ARM_A 3
#define ARM_B 4
#define EJECT_A 5
#define EJECT_B 6

// Digital I/O
#define WINCH_SWITCH 1

// USB ports
#define RIGHT_DRIVE_STICK 1
#define LEFT_DRIVE_STICK 2
#define GAMEPAD_PORT 3

// Left Joystick
#define BUTTON_SHIFT 7

// Gamepad
#define BUTTON_COLLECT 3
#define BUTTON_EJECT 2

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
#define INTAKE_MOTOR Talon // UNKNOWN
#define WINCH_MOTOR Talon // UNKNOWN
#endif // ROBOT_A

#ifdef ROBOT_B
#define ROBOT_NAME "Practice Bot"
#define LEFT_DRIVE_MOTOR Victor
#define RIGHT_DRIVE_MOTOR Victor
#define INTAKE_MOTOR Victor // UNKNOWN
#define WINCH_MOTOR Victor // UNKNOWN
#endif // ROBOT_B
#endif // BASICDEFINES_H_
