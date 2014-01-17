#include "WPILib.h"
#include "BasicDefines.h"
#include "EJoystick.h"
#include "EGamepad.h"

// This is the code for the 2994 2014 robot! :-)
class FRC2994_2014 : public SimpleRobot
{
	// Motor controllers.
	LEFT_DRIVE_MOTOR leftFrontDrive, leftRearDrive;
	RIGHT_DRIVE_MOTOR rightFrontDrive, rightRearDrive;
	INTAKE_MOTOR intake1, intake2;
	WINCH_MOTOR winch;
	
	// Robot drive
	RobotDrive robotDrive;
	
	// USB devices
	EJoystick rightStick;
	EJoystick leftStick;
	EGamepad gamepad;
	
	// Solenoids
	DoubleSolenoid shifters;
	
	// Misc.
	DriverStationLCD *dsLCD;

public:
	FRC2994_2014():
		leftFrontDrive(LEFT_FRONT_DRIVE_PWM),
		leftRearDrive(LEFT_REAR_DRIVE_PWM),
		rightFrontDrive(RIGHT_FRONT_DRIVE_PWM),
		rightRearDrive(RIGHT_REAR_DRIVE_PWM),
		intake1(INTAKE_MOTOR_1_PWM),
		intake2(INTAKE_MOTOR_2_PWM),
		winch(WINCH_MOTOR_PWM),
		robotDrive(&leftFrontDrive, &leftRearDrive, &rightFrontDrive, &rightRearDrive),
		rightStick(RIGHT_DRIVE_STICK),
		leftStick(LEFT_DRIVE_STICK),
		gamepad(GAMEPAD_PORT),
	    shifters(SHIFTER_A, SHIFTER_B)
	{
		// Print an I'M ALIVE message before anything else. NOTHING ABOVE THIS LINE.
		dsLCD = DriverStationLCD::GetInstance();

		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, ROBOT_NAME);
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, __DATE__ " " __TIME__);
		dsLCD->UpdateLCD();

		robotDrive.SetExpiration(0.1);
	}

	// Autonomous
	//	* Code to be run autonomously for the first ten (10) seconds of the match.
	void Autonomous()
	{
		robotDrive.SetSafetyEnabled(false);
	}
	
	// HandleDriverInputs
	//	* Drive motors according to joystick values
	//	* Shift (Button 7 on left joystick)
	//		----> ASSUMES kForward = high gear
	void HandleDriverInputs()
	{
		
	}
	
	// HandleShooter
	//	* Manage winch motor state.
	//	* Toggles collection and eject mode (Gamepad button 4)
	//		----> ASSUMES positive values = collecting
	void HandleShooter()
	{
		
	}
	
	// HandleArm
	//	* Manage solenoids for arm up-down
	void HandleArm()
	{
		
	}
	
	// HandleEject
	//	* Toggle intake motors (in opp. direction)
	void HandleEject() 
	{
		
	}
	
	
	// RegisterButtons
	//	* Register all the buttons required
	void RegisterButtons()
	{
		leftStick.EnableButton(BUTTON_SHIFT);
		gamepad.EnableButton(BUTTON_COLLECT);
		gamepad.EnableButton(BUTTON_EJECT);
	}

	// Code to be run during the remaining 2:20 of the match (after Autonomous())
	//
	// OperatorControl
	//	* Calls all the above methods
	void OperatorControl()
	{
		/* TODO: Investigate. At least year's (GTR East) competition, we reached the conclusion that disabling this was 
		 * the only way we could get out robot code to work (reliably). Should this be set to false?
		 */ 
		robotDrive.SetSafetyEnabled(true);
		
		RegisterButtons();
		while (IsOperatorControl())
		{
			HandleDriverInputs();
			HandleShooter();
			HandleArm();
			HandleEject();
			
			robotDrive.ArcadeDrive(rightStick);
			Wait(0.005);
		}
	}

	// Runs during test mode
	// Test
	// * 
	void Test()
	{

	}
};

START_ROBOT_CLASS(FRC2994_2014);
