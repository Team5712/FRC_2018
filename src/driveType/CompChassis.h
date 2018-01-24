#ifndef SRC_DRIVETYPE_COMPCHASSIS_H_
#define SRC_DRIVETYPE_COMPCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "ctre/Phoenix.h"


//using namespace CTRE::Phoenix::MotorControl::CAN;

class CompChassis : public BaseDrive
{
public:
	CompChassis();
	virtual ~CompChassis();

	// Override DifferentialDrive methods from the base drive class
	// ---- DIFFERENTIAL_DRIVE METHODS ----
	void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) override;
	void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) override;
	void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) override;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

	// ---- ROBOT.CPP METHODS ----
	void autonomousPeriodic() override;
	void teleopPeriodic() override;
	// ---- END ROBOT.CPP METHODS ----

	void driveStraight(double speed) override;

private:
	// Probably have multiple motors, so declare master / slaves here
	WPI_TalonSRX *leftTalonMaster;
	WPI_TalonSRX *rightTalonMaster;

	double leftBias = 0.0; // Bias used to drive straight
	double rightBias = 0.0;

};

#endif /* SRC_DRIVETYPE_COMPCHASSIS_H_ */
