/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "ERobotDrive.h"

#include "CANJaguar.h"
#include "GenericHID.h"
#include "Joystick.h"
#include "Jaguar.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Utility.h"
#include "WPIErrors.h"
#include <math.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))

const int32_t ERobotDrive::kMaxNumberOfMotors;

/*
 * Driving functions
 * These functions provide an interface to multiple motors that is used for C programming
 * The Drive(speed, direction) function is the main part of the set that makes it easy
 * to set speeds and direction independently in one call.
 */

/**
 * Common function to initialize all the robot drive constructors.
 * Create a motor safety object (the real reason for the common code) and
 * initialize all the motor assignments. The default timeout is set for the robot drive.
 */
void ERobotDrive::InitRobotDrive() {
	m_frontLeftMotor = NULL;
	m_centerLeftMotor = NULL;
	m_frontRightMotor = NULL;
	m_rearRightMotor = NULL;
	m_centerRightMotor = NULL;
	m_rearLeftMotor = NULL;
	m_sensitivity = 0.5;
	m_maxOutput = 1.0;
	m_safetyHelper = new MotorSafetyHelper(this);
	m_safetyHelper->SetSafetyEnabled(true);
}

ERobotDrive::ERobotDrive(SpeedController &frontLeftMotor, SpeedController &rearLeftMotor, SpeedController &centerLeftMotor,
						SpeedController &centerRightMotor, SpeedController &frontRightMotor, SpeedController &rearRightMotor)
{
	InitRobotDrive();
	m_frontLeftMotor = &frontLeftMotor;
	m_rearLeftMotor = &rearLeftMotor;
	m_centerLeftMotor = &centerLeftMotor;
	m_centerRightMotor = &centerRightMotor;
	m_frontRightMotor = &frontRightMotor;
	m_rearRightMotor = &rearRightMotor;
	for (int32_t i=0; i < kMaxNumberOfMotors; i++)
	{
		m_invertedMotors[i] = 1;
	}
	m_deleteSpeedControllers = false;
}

/**
 * RobotDrive destructor.
 * Deletes motor objects that were not passed in and created internally only.
 **/
ERobotDrive::~ERobotDrive()
{
	if (m_deleteSpeedControllers)
	{
		delete m_frontLeftMotor;
		delete m_rearLeftMotor;
		delete m_frontRightMotor;
		delete m_rearRightMotor;
		delete m_centerRightMotor;
		delete m_centerLeftMotor;
	}
	delete m_safetyHelper;
}

/**
 * Drive the motors at "speed" and "curve".
 *
 * The speed and curve are -1.0 to +1.0 values where 0.0 represents stopped and
 * not turning. The algorithm for adding in the direction attempts to provide a constant
 * turn radius for differing speeds.
 *
 * This function will most likely be used in an autonomous routine.
 *
 * @param outputMagnitude The forward component of the output magnitude to send to the motors.
 * @param curve The rate of turn, constant for different forward speeds.
 */
void ERobotDrive::Drive(float outputMagnitude, float curve)
{
	float leftOutput, rightOutput;
	static bool reported = false;
	if (!reported)
	{
		nUsageReporting::report(nUsageReporting::kResourceType_RobotDrive, GetNumMotors(), nUsageReporting::kRobotDrive_ArcadeRatioCurve);
		reported = true;
	}

	if (curve < 0)
	{
		float value = log(-curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
		leftOutput = outputMagnitude / ratio;
		rightOutput = outputMagnitude;
	}
	else if (curve > 0)
	{
		float value = log(curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
		leftOutput = outputMagnitude;
		rightOutput = outputMagnitude / ratio;
	}
	else
	{
		leftOutput = outputMagnitude;
		rightOutput = outputMagnitude;
	}
	SetLeftRightMotorOutputs(leftOutput, rightOutput);
}

/**
 * Arcade drive implements single stick driving.
 * Given a single Joystick, the class assumes the Y axis for the move value and the X axis
 * for the rotate value.
 * (Should add more information here regarding the way that arcade drive works.)
 * @param stick The joystick to use for Arcade single-stick driving. The Y-axis will be selected
 * for forwards/backwards and the X-axis will be selected for rotation rate.
 * @param squaredInputs If true, the sensitivity will be increased for small values
 */
void ERobotDrive::ArcadeDrive(GenericHID *stick, bool squaredInputs)
{
	// simply call the full-featured ArcadeDrive with the appropriate values
	ArcadeDrive(stick->GetY(), stick->GetX(), squaredInputs);
}

/**
 * Arcade drive implements single stick driving.
 * Given a single Joystick, the class assumes the Y axis for the move value and the X axis
 * for the rotate value.
 * (Should add more information here regarding the way that arcade drive works.)
 * @param stick The joystick to use for Arcade single-stick driving. The Y-axis will be selected
 * for forwards/backwards and the X-axis will be selected for rotation rate.
 * @param squaredInputs If true, the sensitivity will be increased for small values
 */
void ERobotDrive::ArcadeDrive(GenericHID &stick, bool squaredInputs)
{
	// simply call the full-featured ArcadeDrive with the appropriate values
	ArcadeDrive(stick.GetY(), stick.GetX(), squaredInputs);
}

/**
 * Arcade drive implements single stick driving.
 * Given two joystick instances and two axis, compute the values to send to either two
 * or four motors.
 * @param moveStick The Joystick object that represents the forward/backward direction
 * @param moveAxis The axis on the moveStick object to use for fowards/backwards (typically Y_AXIS)
 * @param rotateStick The Joystick object that represents the rotation value
 * @param rotateAxis The axis on the rotation object to use for the rotate right/left (typically X_AXIS)
 * @param squaredInputs Setting this parameter to true increases the sensitivity at lower speeds
 */
void ERobotDrive::ArcadeDrive(GenericHID* moveStick, uint32_t moveAxis,
								GenericHID* rotateStick, uint32_t rotateAxis,
								bool squaredInputs)
{
	float moveValue = moveStick->GetRawAxis(moveAxis);
	float rotateValue = rotateStick->GetRawAxis(rotateAxis);

	ArcadeDrive(moveValue, rotateValue, squaredInputs);
}

/**
 * Arcade drive implements single stick driving.
 * Given two joystick instances and two axis, compute the values to send to either two
 * or four motors.
 * @param moveStick The Joystick object that represents the forward/backward direction
 * @param moveAxis The axis on the moveStick object to use for fowards/backwards (typically Y_AXIS)
 * @param rotateStick The Joystick object that represents the rotation value
 * @param rotateAxis The axis on the rotation object to use for the rotate right/left (typically X_AXIS)
 * @param squaredInputs Setting this parameter to true increases the sensitivity at lower speeds
 */

void ERobotDrive::ArcadeDrive(GenericHID &moveStick, uint32_t moveAxis,
								GenericHID &rotateStick, uint32_t rotateAxis,
								bool squaredInputs)
{
	float moveValue = moveStick.GetRawAxis(moveAxis);
	float rotateValue = rotateStick.GetRawAxis(rotateAxis);

	ArcadeDrive(moveValue, rotateValue, squaredInputs);
}

/**
 * Arcade drive implements single stick driving.
 * This function lets you directly provide joystick values from any source.
 * @param moveValue The value to use for fowards/backwards
 * @param rotateValue The value to use for the rotate right/left
 * @param squaredInputs If set, increases the sensitivity at low speeds
 */
void ERobotDrive::ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs)
{
	static bool reported = false;
	if (!reported)
	{
		nUsageReporting::report(nUsageReporting::kResourceType_RobotDrive, GetNumMotors(), nUsageReporting::kRobotDrive_ArcadeStandard);
		reported = true;
	}

	// local variables to hold the computed PWM values for the motors
	float leftMotorOutput;
	float rightMotorOutput;

	moveValue = Limit(moveValue);
	rotateValue = Limit(rotateValue);

	if (squaredInputs)
	{
		// square the inputs (while preserving the sign) to increase fine control while permitting full power
		if (moveValue >= 0.0)
		{
			moveValue = (moveValue * moveValue);
		}
		else
		{
			moveValue = -(moveValue * moveValue);
		}
		if (rotateValue >= 0.0)
		{
			rotateValue = (rotateValue * rotateValue);
		}
		else
		{
			rotateValue = -(rotateValue * rotateValue);
		}
	}

	if (moveValue > 0.0)
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = max(moveValue, rotateValue);
		}
		else
		{
			leftMotorOutput = max(moveValue, -rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
	}
	else
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = - max(-moveValue, rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
		else
		{
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = - max(-moveValue, -rotateValue);
		}
	}
	SetLeftRightMotorOutputs(leftMotorOutput, rightMotorOutput);
}

/** Set the speed of the right and left motors.
 * This is used once an appropriate drive setup function is called such as
 * TwoWheelDrive(). The motors are set to "leftOutput" and "rightOutput"
 * and includes flipping the direction of one side for opposing motors.
 * @param leftOutput The speed to send to the left side of the robot.
 * @param rightOutput The speed to send to the right side of the robot.
 */
void ERobotDrive::SetLeftRightMotorOutputs(float leftOutput, float rightOutput)
{
	wpi_assert(m_rearLeftMotor != NULL && m_rearRightMotor != NULL);

	uint8_t syncGroup = 0x80;

	if (m_frontLeftMotor != NULL)
		m_frontLeftMotor->Set(Limit(leftOutput) * m_invertedMotors[kFrontLeftMotor] * m_maxOutput, syncGroup);
	if (m_centerLeftMotor != NULL)
		m_centerLeftMotor->Set(Limit(leftOutput) * m_invertedMotors[kCenterLeftMotor] * m_maxOutput, syncGroup);
	m_rearLeftMotor->Set(Limit(leftOutput) * m_invertedMotors[kRearLeftMotor] * m_maxOutput, syncGroup);
	
	if (m_frontRightMotor != NULL)
		m_frontRightMotor->Set(-Limit(rightOutput) * m_invertedMotors[kFrontRightMotor] * m_maxOutput, syncGroup);
	if (m_centerRightMotor != NULL)
		m_centerRightMotor->Set(-Limit(rightOutput) * m_invertedMotors[kCenterRightMotor] * m_maxOutput, syncGroup);
	m_rearRightMotor->Set(-Limit(rightOutput) * m_invertedMotors[kRearRightMotor] * m_maxOutput, syncGroup);

	CANJaguar::UpdateSyncGroup(syncGroup);

	m_safetyHelper->Feed();
}

/**
 * Limit motor values to the -1.0 to +1.0 range.
 */
float ERobotDrive::Limit(float num)
{
	if (num > 1.0)
	{
		return 1.0;
	}
	if (num < -1.0)
	{
		return -1.0;
	}
	return num;
}

/**
 * Normalize all wheel speeds if the magnitude of any wheel is greater than 1.0.
 */
void ERobotDrive::Normalize(double *wheelSpeeds)
{
	double maxMagnitude = fabs(wheelSpeeds[0]);
	int32_t i;
	for (i=1; i<kMaxNumberOfMotors; i++)
	{
		double temp = fabs(wheelSpeeds[i]);
		if (maxMagnitude < temp) maxMagnitude = temp;
	}
	if (maxMagnitude > 1.0)
	{
		for (i=0; i<kMaxNumberOfMotors; i++)
		{
			wheelSpeeds[i] = wheelSpeeds[i] / maxMagnitude;
		}
	}
}

/**
 * Rotate a vector in Cartesian space.
 */
void ERobotDrive::RotateVector(double &x, double &y, double angle)
{
	double cosA = cos(angle * (3.14159 / 180.0));
	double sinA = sin(angle * (3.14159 / 180.0));
	double xOut = x * cosA - y * sinA;
	double yOut = x * sinA + y * cosA;
	x = xOut;
	y = yOut;
}

/*
 * Invert a motor direction.
 * This is used when a motor should run in the opposite direction as the drive
 * code would normally run it. Motors that are direct drive would be inverted, the
 * Drive code assumes that the motors are geared with one reversal.
 * @param motor The motor index to invert.
 * @param isInverted True if the motor should be inverted when operated.
 */
void ERobotDrive::SetInvertedMotor(MotorType motor, bool isInverted)
{
	if (motor < 0 || motor > 3)
	{
		wpi_setWPIError(InvalidMotorIndex);
		return;
	}
	m_invertedMotors[motor] = isInverted ? -1 : 1;
}

/**
 * Set the turning sensitivity.
 * 
 * This only impacts the Drive() entry-point.
 * @param sensitivity Effectively sets the turning sensitivity (or turn radius for a given value)
 */
void ERobotDrive::SetSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

/**
 * Configure the scaling factor for using RobotDrive with motor controllers in a mode other than PercentVbus.
 * @param maxOutput Multiplied with the output percentage computed by the drive functions.
 */
void ERobotDrive::SetMaxOutput(double maxOutput)
{
	m_maxOutput = maxOutput;
}



void ERobotDrive::SetExpiration(float timeout)
{
	m_safetyHelper->SetExpiration(timeout);
}

float ERobotDrive::GetExpiration()
{
	return m_safetyHelper->GetExpiration();
}

bool ERobotDrive::IsAlive()
{
	return m_safetyHelper->IsAlive();
}

bool ERobotDrive::IsSafetyEnabled()
{
	return m_safetyHelper->IsSafetyEnabled();
}

void ERobotDrive::SetSafetyEnabled(bool enabled)
{
	m_safetyHelper->SetSafetyEnabled(enabled);
}

void ERobotDrive::GetDescription(char *desc)
{
	sprintf(desc, "RobotDrive");
}

void ERobotDrive::StopMotor()
{
	if (m_frontLeftMotor != NULL) m_frontLeftMotor->Disable();
	if (m_frontRightMotor != NULL) m_frontRightMotor->Disable();
	if (m_rearLeftMotor != NULL) m_rearLeftMotor->Disable();
	if (m_rearRightMotor != NULL) m_rearRightMotor->Disable();
	if (m_centerLeftMotor != NULL) m_centerLeftMotor->Disable();
	if (m_centerRightMotor != NULL) m_centerRightMotor->Disable();
}
