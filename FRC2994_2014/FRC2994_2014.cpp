#include "WPILib.h"
#include "BasicDefines.h"
#include "EJoystick.h"
#include "EGamepad.h"
#include "EDigitalInput.h"

// This is the code for the 2994 2014 robot! :-)
class FRC2994_2014 : public SimpleRobot
{
	// Motor controllers.
	LEFT_DRIVE_MOTOR leftFrontDrive, leftRearDrive;
	RIGHT_DRIVE_MOTOR rightFrontDrive, rightRearDrive;
	INTAKE_MOTOR intake;
	WINCH_MOTOR winch;

	// Robot drive
	RobotDrive robotDrive;

	// USB devices
	EJoystick rightStick;
	EJoystick leftStick;
	EGamepad gamepad;

	// Solenoids
	DoubleSolenoid shifters;
	DoubleSolenoid arm;
	DoubleSolenoid eject; 

	// Digital IOs
	EDigitalInput winchSwitch;
	Encoder leftDriveEncoder, rightDriveEncoder;

	// Misc.
	Timer ejectTimer;
	DriverStationLCD *dsLCD;
	Compressor compressor;

	bool loaded;
	bool loading;
	bool armDown;

public:
	FRC2994_2014():
		leftFrontDrive(LEFT_FRONT_DRIVE_PWM),
		leftRearDrive(LEFT_REAR_DRIVE_PWM),
		rightFrontDrive(RIGHT_FRONT_DRIVE_PWM),
		rightRearDrive(RIGHT_REAR_DRIVE_PWM),
		intake(INTAKE_MOTOR_PWM),
		winch(WINCH_MOTOR_PWM),
		robotDrive(&leftFrontDrive, &leftRearDrive, &rightFrontDrive, &rightRearDrive),
		rightStick(RIGHT_DRIVE_STICK),
		leftStick(LEFT_DRIVE_STICK),
		gamepad(GAMEPAD_PORT),
		shifters(SHIFTER_A, SHIFTER_B),
		arm(ARM_A, ARM_B),
		eject(EJECT_A, EJECT_B),
		winchSwitch(WINCH_SWITCH),
		leftDriveEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B),
		rightDriveEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B),
		compressor(COMPRESSOR_PRESSURE_SW, COMPRESSOR_SPIKE),
		// Robot starts off in a loaded state so a ball can be fit in
		loaded(true),
		loading(false),
		armDown(false)
	{
		// Print an I'M ALIVE message before anything else. NOTHING ABOVE THIS LINE.
		dsLCD = DriverStationLCD::GetInstance();

		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, ROBOT_NAME);
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, __DATE__ " " __TIME__);
		dsLCD->UpdateLCD();

		// Set the experation to 1.5 times the loop speed.
		robotDrive.SetExpiration(LOOP_PERIOD*1.5);
	}
	
	// CheckLoad
	//	* Checks if the winch switch has been pressed.
	//		----> If yes, makes sure the state of the motors and variables is correct.
	//	* Should be called every tick of an operator control loop to ensure that
	//	  the motor is shut off as soon as possible.
	void CheckLoad()
	{
		// Switch is normally closed
		if (loading && !winchSwitch.Get()) 
		{
			winch.Set(0.0);
			loaded = true;
			loading = false;
		}
	}
	
	// InitiateLoad
	//	* Begins a load of the catapult by running the winch motor.
	void InitiateLoad()
	{
		if (!loaded)
		{
			winch.Set(WINCH_FWD);
			loading = true;
		}
	}
	
	// LaunchCatapult
	//	* If in the correct state to launch (loaded), launches the catapult.
	void LaunchCatapult()
	{
		if (loaded)
		{
			winch.Set(WINCH_FWD);
			Wait(CATAPULT_SHOOT_WAIT);
			winch.Set(0.0);
			loaded = false;
		}
	}

	// Autonomous
	//	* Code to be run autonomously for the first ten (10) seconds of the match.
	//	* Launch catapult
	//	* Drive robot forward ENCODER_DIST ticks.
	void Autonomous()
	{
		robotDrive.SetSafetyEnabled(false);
		
		LaunchCatapult();

		leftDriveEncoder.Reset();
		int dist = ENCODER_DIST;
		int reading = 0;
		// The encoder.Reset() method seems not to set Get() values back to zero,
		// so we use a variable to capture the initial value.
		int initial = leftDriveEncoder.Get();
		
		// Start moving the robot
		robotDrive.Drive(AUTO_DRIVE_SPEED, 0.0);

		while (IsAutonomous() && (reading <= dist))
		{
			reading = (leftDriveEncoder.Get() - initial);
		}
	}

	// HandleDriverInputs
	//	* Drive motors according to joystick values
	//	* Shift (Button 7 on left joystick)
	//		----> ASSUMES kForward = high gear
	void HandleDriverInputs()
	{
		if(kEventOpened == leftStick.GetEvent(BUTTON_SHIFT))
		{
			// Shift into high gear.
			shifters.Set(DoubleSolenoid::kForward);
		}
		else if(kEventClosed == leftStick.GetEvent(BUTTON_SHIFT))
		{
			// Shift into low gear.
			shifters.Set(DoubleSolenoid::kReverse);
		}

		robotDrive.ArcadeDrive(rightStick);
	}

	// HandleShooter
	//	* Manage winch motor state.
	//	* Toggles collection and eject mode (Gamepad button 4)
	//		----> ASSUMES positive values = collecting
	void HandleShooter()
	{
		if (gamepad.GetEvent(BUTTON_LOAD) == kEventClosed) 
		{
			InitiateLoad();
		}
		if (loading) 
		{
			CheckLoad();
		}
		if (gamepad.GetEvent(BUTTON_SHOOT) == kEventClosed) 
		{
			LaunchCatapult();
		}
	}

	// HandleArm
	//	* Manage solenoids for arm up-down
	//		----> ASSUMES kForward on DoubleSolenoid is the down position.
	//	* Handle intake motors
	void HandleArm()
	{
		if (gamepad.GetEvent(BUTTON_ARM) == kEventClosed && armDown)
		{
			arm.Set(DoubleSolenoid::kReverse);
		}
		else if (gamepad.GetEvent(BUTTON_ARM) == kEventClosed)
		{
			arm.Set(DoubleSolenoid::kForward);
		}

		if (gamepad.GetDPadEvent(EGamepad::kUp) == kEventClosed)
		{
			intake.Set(1.0);
		}
		else if (gamepad.GetDPadEvent(EGamepad::kUp) == kEventOpened)
		{
			intake.Set(0.0);
		}
		if(gamepad.GetDPadEvent(EGamepad::kDown) == kEventClosed)
		{
			intake.Set(-1.0);
		}
		else if (gamepad.GetDPadEvent(EGamepad::kDown) == kEventOpened)
		{
			intake.Set(0.0);
		}
	}

	// HandleEject
	//	* Handle eject piston.
	void HandleEject() 
	{
		if (gamepad.GetEvent(BUTTON_PASS) == kEventClosed)
		{
			ejectTimer.Start();
			eject.Set(DoubleSolenoid::kForward);
		}
		if (ejectTimer.HasPeriodPassed(EJECT_WAIT))
		{
			ejectTimer.Stop();
			ejectTimer.Reset();
			eject.Set(DoubleSolenoid::kReverse);
		}
	}


	// RegisterButtons
	//	* Register all the buttons required
	void RegisterButtons()
	{
		leftStick.EnableButton(BUTTON_SHIFT);
		gamepad.EnableButton(BUTTON_LOAD);
		gamepad.EnableButton(BUTTON_SHOOT);
		gamepad.EnableButton(BUTTON_ARM);
		gamepad.EnableButton(BUTTON_PASS);
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

		Timer clock;
		int sanity = 0;
		int bigSanity = 0;

		RegisterButtons();
		gamepad.Update();
		leftStick.Update();
		
		compressor.Start();

		while (IsOperatorControl())
		{
			clock.Start();

			HandleDriverInputs();
			HandleShooter();
			HandleArm();
			HandleEject();

			while (!clock.HasPeriodPassed(LOOP_PERIOD));
			clock.Reset();
			sanity++;
			if (sanity >= 100)
			{
				bigSanity++;
				sanity = 0;
				dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "%d", bigSanity);
			}
			gamepad.Update();
			leftStick.Update();
			dsLCD->UpdateLCD();
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
