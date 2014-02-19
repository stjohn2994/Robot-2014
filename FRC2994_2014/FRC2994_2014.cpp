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
		
		leftDriveEncoder.SetReverseDirection(true);
	}
	
	// CheckLoad
	//	* Checks if the winch switch has been pressed.
	//		----> If yes, makes sure the state of the motors and variables is correct.
	//	* Should be called every tick of an operator control loop to ensure that
	//	  the motor is shut off as soon as possible.
	bool CheckLoad()
	{
		// Switch is normally closed
		if (loading && winchSwitch.Get()) 
		{
			winch.Set(0.0);
			loaded = true;
			loading = false;
			return false;
		}
		
		return true;
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
		// SAFETY AND SANITY - SET ALL TO ZERO
		intake.Set(0.0);
		winch.Set(0.0);
		
//		robotDrive.SetSafetyEnabled(false);
		
//		LaunchCatapult();
//		
//		Wait(1.0);
//		
//		leftDriveEncoder.Start();
//		leftDriveEncoder.Reset();
//		int dist = ENCODER_DIST;
//		int reading = 0;
//		// The encoder.Reset() method seems not to set Get() values back to zero,
//		// so we use a variable to capture the initial value.
//		int initial = leftDriveEncoder.Get();
//		
//		// Start moving the robot
//		robotDrive.Drive(-AUTO_DRIVE_SPEED, 0.0);
//
//		while (IsAutonomous() && (reading <= dist))
//		{
//			reading = (leftDriveEncoder.Get() - initial);
//		}
//		
//		robotDrive.Drive(0.0, 0.0);
//		
//		leftDriveEncoder.Stop();
		
		loaded = winchSwitch.Get();
				
		loading = !loaded;
		
		robotDrive.SetSafetyEnabled(false);
		
		LaunchCatapult();
		
		Wait(AUTO_SHOOT_WAIT);
		
		InitiateLoad();
		
		if (loaded == true) {
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "BLAME JACK IT FAILED");
			dsLCD->UpdateLCD();
			return;
		}
		
		intake.Set(1.0);
		
		while (CheckLoad());
		
		if (loading == true) {
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "BLAME JACK IT FAILED");
			dsLCD->UpdateLCD();
			return;
		}
		
		arm.Set(DoubleSolenoid::kForward);
		
		Wait(AUTO_PICKUP_WAIT);
		
		intake.Set(0.0);
		arm.Set(DoubleSolenoid::kReverse);
		
		LaunchCatapult();
		
		Wait(AUTO_SHOOT_WAIT);
		
		leftDriveEncoder.Start();
		leftDriveEncoder.Reset();
		int dist = ENCODER_DIST;
		int reading = 0;
		// The encoder.Reset() method seems not to set Get() values back to zero,
		// so we use a variable to capture the initial value.
		int initial = leftDriveEncoder.Get();
		
		// Start moving the robot
		robotDrive.Drive(-AUTO_DRIVE_SPEED, 0.0);

		while (IsAutonomous() && (reading <= dist))
		{
			reading = (leftDriveEncoder.Get() - initial);
		}
		
		robotDrive.Drive(0.0, 0.0);
		
		reading = leftDriveEncoder.Get();
		dist = reading - ENCODER_BACK_DIST;
		
		robotDrive.Drive(AUTO_DRIVE_SPEED, 0.0);
		
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "b: %d", reading);
		dsLCD->UpdateLCD();
		
		while (IsAutonomous() && (reading >= dist)) {
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "%d", reading);
			dsLCD->UpdateLCD();
			reading = (leftDriveEncoder.Get());
		}
		
		robotDrive.Drive(0.0, 0.0);
		
		leftDriveEncoder.Stop();
		
		// SAFETY AND SANITY - SET ALL TO ZERO
		intake.Set(0.0);
		winch.Set(0.0);
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

		robotDrive.ArcadeDrive(rightStick.GetY(), -rightStick.GetX());
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
			armDown = false;
		}
		else if (gamepad.GetEvent(BUTTON_ARM) == kEventClosed)
		{
			arm.Set(DoubleSolenoid::kForward);
			armDown = true;
		}

		if (gamepad.GetDPadEvent(BUTTON_INTAKE_FWD) == kEventClosed)
		{
			intake.Set(-0.85);
		}
		else if (gamepad.GetDPadEvent(BUTTON_INTAKE_FWD) == kEventOpened)
		{
			intake.Set(0.0);
		}
		
		if(gamepad.GetDPadEvent(BUTTON_INTAKE_BWD) == kEventClosed)
		{
			intake.Set(0.85);
		}
		if (gamepad.GetDPadEvent(BUTTON_INTAKE_BWD) == kEventOpened)
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
		// SAFETY AND SANITY - SET ALL TO ZERO
		intake.Set(0.0);
		winch.Set(0.0);
		
		arm.Set(DoubleSolenoid::kReverse);
		
		/* TODO: Investigate. At least year's (GTR East) competition, we reached the conclusion that disabling this was 
		 * the only way we could get out robot code to work (reliably). Should this be set to false?
		 */ 
		robotDrive.SetSafetyEnabled(true);

		Timer clock;
		int sanity = 0;
		int bigSanity = 0;
		
		loading = false;
		loaded = winchSwitch.Get();

		RegisterButtons();
		gamepad.Update();
		leftStick.Update();
		
		compressor.Start();

		while (IsOperatorControl()&& IsEnabled())
		{
			clock.Start();

			HandleDriverInputs();
			HandleShooter();
			HandleArm();
//			HandleEject();

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
		
		// SAFETY AND SANITY - SET ALL TO ZERO
		intake.Set(0.0);
		winch.Set(0.0);
	}

	// Runs during test mode
	// Test
	// * 
	void Test()
	{
		shifters.Set(DoubleSolenoid::kForward);
		
		leftDriveEncoder.Start();
		leftDriveEncoder.Reset();
		
		int start = leftDriveEncoder.Get();
		
		while (IsTest()) {
			if (rightStick.GetRawButton(7)) {
				robotDrive.ArcadeDrive(rightStick.GetY(), -rightStick.GetX());
			}
			else {
				robotDrive.ArcadeDrive(rightStick.GetY()/2, -rightStick.GetX()/2);
			}
			
			if (gamepad.GetEvent(4) == kEventClosed) {
				start = leftDriveEncoder.Get();
			}
			
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "lde: %d", leftDriveEncoder.Get() - start);
			dsLCD->UpdateLCD();
			
			gamepad.Update();
		}
	}
};

START_ROBOT_CLASS(FRC2994_2014);
