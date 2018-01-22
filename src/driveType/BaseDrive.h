#ifndef SRC_DRIVETYPE_BASEDRIVE_H_
#define SRC_DRIVETYPE_BASEDRIVE_H_

// Include statements from most local to most global

#include "ctre/Phoenix/MotorControl/CAN/TalonSRX.h"
#include "WPILib.h"

//using namespace CTRE::MotorControl::CAN;

class BaseDrive
{
public:

	// periodic function that both the child classes will implement
	virtual void teleopPeriodic() = 0;

protected:
	// These arrays will store the port mappings for the motor(s)
	int LEFT_MOTOR_PORTS; // Will act as an array in child classes
	int RIGHT_MOTOR_PORTS;

	DifferentialDrive *drive;
//	TalonSRX *leftTalons[]; // List of left, right motors
//	TalonSRX *rightTalons[];

	Joystick *joystick;

private:


};

#endif /* SRC_DRIVETYPE_BASEDRIVE_H_ */
