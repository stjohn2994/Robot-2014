#include "WPILib.h"
#include "BasicDefines.h"
#include "EJoystick.h"

/*
 * This is the code for the 2994 2014 robot! :-)
 */
class FRC2994_2014 : public SimpleRobot
{
	LEFT_DRIVE_MOTOR leftFrontDrive, leftRearDrive;
	RIGHT_DRIVE_MOTOR rightFrontDrive, rightRearDrive;
	
	RobotDrive robotDrive;
	EJoystick rightStick;
	DriverStationLCD *dsLCD;

public:
	FRC2994_2014():
		leftFrontDrive(LEFT_FRONT_DRIVE_PWM),
		leftRearDrive(LEFT_REAR_DRIVE_PWM),
		rightFrontDrive(RIGHT_FRONT_DRIVE_PWM),
		rightRearDrive(RIGHT_REAR_DRIVE_PWM),
		robotDrive(&leftFrontDrive, &leftRearDrive, &rightFrontDrive, &rightRearDrive),
		rightStick(RIGHT_DRIVE_STICK)
	{
		// Print an I'M ALIVE message before anything else. NOTHING ABOVE THIS LINE.
		dsLCD = DriverStationLCD::GetInstance();

		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, ROBOT_NAME);
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, __DATE__ " " __TIME__);
		dsLCD->UpdateLCD();

		robotDrive.SetExpiration(0.1);
	}

	/**
	 * Code to be run autonomously for the first ten (10) seconds of the match.
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
