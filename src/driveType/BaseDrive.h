#ifndef SRC_DRIVETYPE_BASEDRIVE_H_
#define SRC_DRIVETYPE_BASEDRIVE_H_

#if true
	#define CTRE ctre
	#define Phoenix phoenix
	#define MotorControl motorcontrol
	#define CAN can
#endif

// Include statements from most local to most global

#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "WPILib.h"

using namespace CTRE::Phoenix::MotorControl::CAN;

class BaseDrive
{
public:
	virtual ~BaseDrive() = 0;

	// ---- DIFFERENTIAL_DRIVE METHODS ----
	virtual void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) = 0;
	virtual void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) = 0;
	virtual void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) = 0;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

	virtual void driveStraight(double speed) = 0;
	virtual double getJoystickValue(int axisNum) = 0;

protected:
//	// These ARRAYS will store the port mappings for the motor(s)
//	const int *LEFT_MOTOR_PORTS; // Will act as an array in child classes
//	const int *RIGHT_MOTOR_PORTS;

	DifferentialDrive *drive;
//	std::vector<TalonSRX> leftTalons; // List of left, right motors
//	std::vector<TalonSRX> rightTalons;

	Joystick *joystick;

private:


};

#endif /* SRC_DRIVETYPE_BASEDRIVE_H_ */
