#ifndef SRC_DRIVETYPE_PEANUTCHASSIS_H_
#define SRC_DRIVETYPE_PEANUTCHASSIS_H_

#include <driveType/BaseDrive.hpp>
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

	// ---- ROBOT.CPP METHODS ----
	void autonomousInit() override;
	void autonomousPeriodic() override;
	void teleopInit() override;
	void teleopPeriodic() override;
	// ---- END ROBOT.CPP METHODS ----

	// Overwritten from BaseDrive.h
	void driveStraight(double speed) override;
	float getRatio() override;
	double* getEncoderValues() override; // Pointer will be used as an array


private:

	WPI_TalonSRX *leftTalon;
	WPI_TalonSRX *rightTalon;

	Timer *timer;
	Potentiometer *pot;

	// Values specific to this class
	double leftBias = 0.0; // Used to drive straight. Pre-defined and tested
	double rightBias = 0.0;
	int distance = 18.85*10;
	int leftTraveled = 0;
	int rightTraveled = 0;

	// Constants
	unsigned const int TICKS = 515;
	unsigned const int WHEEL_RADIUS = 3;
	const float RATIO = (TICKS / (2 * M_PI * WHEEL_RADIUS));
//	const double leftRatio = (6*M_PI)/505;
//	const double rightRatio = (6*M_PI)/505;

};

#endif /* SRC_DRIVETYPE_PEANUTCHASSIS_H_ */
