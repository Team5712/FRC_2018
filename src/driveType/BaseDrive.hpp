#ifndef SRC_DRIVETYPE_BASEDRIVE_HPP_
#define SRC_DRIVETYPE_BASEDRIVE_HPP_

#if false
	#define CTRE ctre
	#define Phoenix phoenix
	#define MotorControl motorcontrol
	#define CAN can
#endif

// Include statements from most local to most global

#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "WPILib.h"
#include "AHRS.h"

//using namespace CTRE::Phoenix::MotorControl::CAN;

class BaseDrive
{
public:
	virtual ~BaseDrive()
	{
		delete drive;
		delete joystick;
	};

	// ---- DIFFERENTIAL_DRIVE METHODS ----
	virtual void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) = 0;
	virtual void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) = 0;
	virtual void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) = 0;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

	// ---- ROBOT.CPP METHODS ----
	virtual void autonomousInit() = 0;
	virtual void autonomousPeriodic() = 0;
	virtual void teleopInit() = 0;
	virtual void teleopPeriodic() = 0;
	// ---- END ROBOT.CPP METHODS ----

	virtual void driveStraight(double speed) = 0;

	/**
	 * This will return the value of the axis specified by the given index.
	 * The axis indices start at 0.
	 *
	 * @param axisNum
	 * the index of the desired axis value (0: twist, 1: x, 2: y)
	 *
	 * @return
	 * A double value from -1.0 to +1.0 according to the joystick's position
	 * and the requested axis.
	 */
	virtual double getJoystickValue(int axisNum)
	{
		// TODO: Verify the axis mappings (what is 0, 1, 2, etc.?)
		return joystick->GetRawAxis(axisNum);
	}

protected:
//	// These ARRAYS will store the port mappings for the motor(s)
//	const int *LEFT_MOTOR_PORTS; // Will act as an array in child classes
//	const int *RIGHT_MOTOR_PORTS;

	DifferentialDrive *drive;
//	std::vector<TalonSRX> leftTalons; // List of left, right motors
//	std::vector<TalonSRX> rightTalons;

	Joystick *joystick;

	AHRS *gyro;



private:


};

#endif /* SRC_DRIVETYPE_BASEDRIVE_HPP_ */
