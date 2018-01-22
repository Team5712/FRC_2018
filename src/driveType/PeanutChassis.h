#ifndef SRC_DRIVETYPE_PEANUTCHASSIS_H_
#define SRC_DRIVETYPE_PEANUTCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "ctre/Phoenix.h"

//using namespace CTRE::Phoenix::MotorControl::CAN;


class PeanutChassis : public BaseDrive
{
public:
	PeanutChassis();
	void teleopPeriodic();
	virtual ~PeanutChassis();

	// Override DifferentialDrive methods from the base drive class
	// ---- DIFFERENTIAL_DRIVE METHODS ----
	void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) override;
	void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) override;
	void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) override;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

	// Overwritten from BaseDrive.h
	void driveStraight(double speed) override;

private:

	WPI_TalonSRX *talonLeft;
	WPI_TalonSRX *talonRight;

	// Values specific to this class
	double leftBias = 0.0; // Used to drive straight. Pre-defined and tested
	double rightBias = 0.0;

};

#endif /* SRC_DRIVETYPE_PEANUTCHASSIS_H_ */
