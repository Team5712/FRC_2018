#ifndef SRC_DRIVETYPE_COMPCHASSIS_H_
#define SRC_DRIVETYPE_COMPCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "ctre/Phoenix.h"
#include <math.h>


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
	void autonomousInit() override;
	void autonomousPeriodic() override;
	void teleopInit() override;
	void teleopPeriodic() override;
	// ---- END ROBOT.CPP METHODS ----

	// Overritten from BaseDrive.hpp
	void driveStraight(double speed) override;
	float getRatio() override;

private:
	// Probably have multiple motors, so declare master / slaves here
	WPI_TalonSRX *leftTalonMaster;
	WPI_TalonSRX *rightTalonMaster;

	double leftBias = 0.0; // Bias used to drive straight
	double rightBias = 0.0;

	// Constants
	unsigned const int TICKS = 515;
	unsigned const int WHEEL_RADIUS = 2.1875;
	const float RATIO = (TICKS / (2 * M_PI * WHEEL_RADIUS));

};

#endif /* SRC_DRIVETYPE_COMPCHASSIS_H_ */
