#include "WPILib.h"
#include "BasicDefines.h"

/*
 * This is the code for the 2994 2014 robot! :-)
 */
class FRC2994_2014 : public SimpleRobot
{
	Talon leftFrontDrive, leftRearDrive;
	Talon rightFrontDrive, rightRearDrive;
	
	RobotDrive robotDrive;
	Joystick rightStick;

public:
	FRC2994_2014():
		leftFrontDrive(LEFT_FRONT_DRIVE_PWM),
		leftRearDrive(LEFT_REAR_DRIVE_PWM),
		rightFrontDrive(RIGHT_FRONT_DRIVE_PWM),
		rightRearDrive(RIGHT_REAR_DRIVE_PWM),
		robotDrive(&leftFrontDrive, &leftRearDrive, &rightFrontDrive, &rightRearDrive),
		rightStick(RIGHT_DRIVE_STICK)
	{
		robotDrive.SetExpiration(0.1);
	}

	/**
	 * Code to be run autonomously for the first ten seconds of the match.
	 */
	void Autonomous()
	{
		robotDrive.SetSafetyEnabled(false);
	}

	/**
	 * Code to be run during the remaining 2:20 of the match (after Autonomous())
	 */
	void OperatorControl()
	{
		/* TODO: Investigate. At least year's (GTR East) competition, we reached the conclusion that disabling this was 
		 * the only way we could get out robot code to work (reliably). Should this be set to false?
		 */ 
		robotDrive.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			robotDrive.ArcadeDrive(rightStick);
			Wait(0.005);
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test()
	{

	}
};

START_ROBOT_CLASS(FRC2994_2014);
