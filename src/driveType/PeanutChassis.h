#ifndef SRC_DRIVETYPE_PEANUTCHASSIS_H_
#define SRC_DRIVETYPE_PEANUTCHASSIS_H_

#include "BaseDrive.h"
#include "WPILib.h"
#include <iostream>
#include <string>
#include <cmath>

#include <IterativeRobot.h>
#include "ctre/Phoenix.h"
#include "Drive/DifferentialDrive.h"
#include "Joystick.h"

class PeanutChassis : public BaseDrive
{
public:
	PeanutChassis();
	void teleopPeriodic();
	virtual ~PeanutChassis();


private:
	WPI_TalonSRX *talonLeft;
	WPI_TalonSRX *talonRight;

};

#endif /* SRC_DRIVETYPE_PEANUTCHASSIS_H_ */
