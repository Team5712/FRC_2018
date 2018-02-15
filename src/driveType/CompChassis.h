#ifndef SRC_DRIVETYPE_COMPCHASSIS_H_
#define SRC_DRIVETYPE_COMPCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "../Constants.h"
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

	void autonomousInit() override;
	void autonomousPeriodic() override;
	void teleopInit() override;
	void teleopPeriodic() override;

	void handleInput();

	WPI_TalonSRX *l_master;
	WPI_TalonSRX *r_master;

	// ---- END DIFFERENTIAL_DRIVE METHODS ----

//	// DEPRECATED
//	// ---- ROBOT.CPP METHODS ----
//	void autonomousInit() override;
//	void autonomousPeriodic() override;
//	void teleopInit() override;
//	void teleopPeriodic() override;
//	// ---- END ROBOT.CPP METHODS ----

	// Overritten from BaseDrive.hpp
	void driveStraight(double speed) override;
	int* getEncoderValues() override; // Pointer will be used as an array
	float getGyroYaw() override;


private:
	// Probably have multiple motors, so declare master / slaves here


	WPI_VictorSPX *l_slave1;
	WPI_VictorSPX *l_slave2;
	WPI_VictorSPX *l_slave3;

	WPI_VictorSPX *r_slave1;
	WPI_VictorSPX *r_slave2;
	WPI_VictorSPX *r_slave3;

	WPI_VictorSPX *l_intake;
	WPI_VictorSPX *r_intake;

	WPI_TalonSRX *lift_master;

	AnalogPotentiometer *pot;

	Spark *led;

	bool lift_isFloor = false;
	bool lift_isSwitch = false;
	bool lift_isScale = false;

	double lift_potScale = 0.999845;
	double lift_potSwitch = 1.00143;
	double lift_potFloor = 1.00163;

	double current_led =  0.99;

	int led_current = 0;
	float led_values[100];



};

#endif /* SRC_DRIVETYPE_COMPCHASSIS_H_ */
