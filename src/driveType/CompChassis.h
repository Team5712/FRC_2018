#ifndef SRC_DRIVETYPE_COMPCHASSIS_H_
#define SRC_DRIVETYPE_COMPCHASSIS_H_

#include "BaseDrive.h"
//#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "ctre/Phoenix.h"


using namespace CTRE::Phoenix::MotorControl::CAN;

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


private:
	// Probably have multiple motors, so declare master / slaves here
	WPI_TalonSRX *leftTalonMaster;
	WPI_TalonSRX *rightTalonMaster;

};

#endif /* SRC_DRIVETYPE_COMPCHASSIS_H_ */
