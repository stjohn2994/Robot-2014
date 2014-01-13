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

// USB ports
#define RIGHT_DRIVE_STICK 1
#define LEFT_DRIVE_STICK 2
#define GAMEPAD_PORT 3

#ifdef PLYBOY
#define ROBOT_NAME "PLYBOY"
#define LEFT_DRIVE_MOTOR Jaguar
#define RIGHT_DRIVE_MOTOR Jaguar
#endif // PLYBOY

#ifdef ROBOT_A
#define ROBOT_NAME "Comp. Bot"
#define LEFT_DRIVE_MOTOR Talon
#define RIGHT_DRIVE_MOTOR Talon
#endif // ROBOT_A

#ifdef ROBOT_B
#define ROBOT_NAME "Practice Bot"
#define LEFT_DRIVE_MOTOR Victor
#define RIGHT_DRIVE_MOTOR Victor
#endif // ROBOT_B
#endif // BASICDEFINES_H_
