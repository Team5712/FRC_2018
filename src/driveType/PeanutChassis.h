#ifndef SRC_DRIVETYPE_PEANUTCHASSIS_H_
#define SRC_DRIVETYPE_PEANUTCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "../Constants.h"
#include "ctre/Phoenix.h"

//using namespace CTRE::Phoenix::MotorControl::CAN;


class PeanutChassis : public BaseDrive
{
public:
	PeanutChassis();
	virtual ~PeanutChassis();

	// Override DifferentialDrive methods from the base drive class
	// ---- DIFFERENTIAL_DRIVE METHODS ----
	void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true) override;
	void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn) override;
	void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true) override;
	// ---- END DIFFERENTIAL_DRIVE METHODS ----

//	// DEPRECATED:
//	// ---- ROBOT.CPP METHODS ----
//	void autonomousInit() override;
//	void autonomousPeriodic() override;
//	void teleopInit() override;
//	void teleopPeriodic() override;
//	// ---- END ROBOT.CPP METHODS ----

	// Overwritten from BaseDrive.h
	void driveStraight(double speed) override;
	double* getEncoderValues() override; // Pointer will be used as an array
	float getGyroYaw() override;

	void autonomousInit() override;
	void autonomousPeriodic() override;
	void teleopInit() override;
	void teleopPeriodic() override;


private:

	WPI_TalonSRX *leftTalon;
	WPI_TalonSRX *rightTalon;

	Timer *timer;

	int distance = 18.85*10;
	int leftTraveled = 0;
	int rightTraveled = 0;

	// Constants


};

#endif /* SRC_DRIVETYPE_PEANUTCHASSIS_H_ */
