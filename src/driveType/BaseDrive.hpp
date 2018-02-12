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
#include "AHRS.h"
#include "WPILib.h"
#include <iostream>

//using namespace CTRE::Phoenix::MotorControl::CAN;

class BaseDrive
{
public:
	virtual ~BaseDrive()
	{
		delete drive;
	};

	// ---- DIFFERENTIAL_DRIVE METHODS ----
	virtual void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) = 0;
	virtual void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) = 0;
	virtual void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) = 0;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

//	// DEPRECATED:
//	// ---- ROBOT.CPP METHODS ----
//	virtual void autonomousInit() = 0;
//	virtual void autonomousPeriodic() = 0;
//	virtual void teleopInit() = 0;
//	virtual void teleopPeriodic() = 0;
//	// ---- END ROBOT.CPP METHODS ----

	virtual void driveStraight(double speed) = 0;
	virtual double* getEncoderValues() = 0; // Pointer will be used as an array
	virtual float getGyroYaw() = 0;

	virtual void autonomousInit() = 0;
	virtual void autonomousPeriodic() = 0;
	virtual void teleopInit() = 0;
	virtual void teleopPeriodic() = 0;


	AHRS *gyro;
	AnalogPotentiometer *pot;

protected:
//	// These ARRAYS will store the port mappings for the motor(s)
//	const int *LEFT_MOTOR_PORTS; // Will act as an array in child classes
//	const int *RIGHT_MOTOR_PORTS;

	DifferentialDrive *drive;
//	std::vector<TalonSRX> leftTalons; // List of left, right motors
//	std::vector<TalonSRX> rightTalons;

	Joystick *joystick_l;
	Joystick *joystick_r;
	Joystick *joystick_lift;

	// peanut chassis
	// radius - 3 in
	// ticks / revolution - 515

	// competition chassis
	// radius - 2 3/16 in

	// multiply ratio by the distance in inches you want to go
	// (ticks / 2r*pi) * distance

private:


};

#endif /* SRC_DRIVETYPE_BASEDRIVE_HPP_ */
