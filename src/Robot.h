#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

// Order imports based on most local -> global

#include <autonomousModes/AutoBase.hpp>
#include "driveType/BaseDrive.hpp"
#include "driveType/PeanutChassis.h"
#include "driveType/CompChassis.h"
#include "autonomousModes/AutoLeft.h"
#include "autonomousModes/AutoMiddle.h"
#include "autonomousModes/AutoRight.h"
#include "StartingPosition.h"
#include "SDInterface.h"
#include "WPILib.h"
#include <iostream>
//#include <iostream>
//#include <string>
//#include <iostream>
//#include <string>
//#include <cmath>
//
//#include <IterativeRobot.h>
//#include "ctre/Phoenix.h"
//#include "Drive/DifferentialDrive.h"
//#include "./driveType/CompetitionChassis.h"
//#include "./driveType/PeanutChassis.h"
//#include "./driveType/BaseDrive.h"
//#include "Joystick.h"

class Robot: public frc::IterativeRobot {
public:
	Robot();
	~Robot();

	StartingPosition current_position = StartingPosition::LEFT;

	// Basic methods
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	// Unneeded methods
//	void DisabledInit();
//	void DisabledPeriodic();
//	void TestInit();
//	void TestPeriodic();

private:
	AutoBase *autoMode;
	SDInterface sdinterface;
	BaseDrive *drive;

};

#endif /* SRC_ROBOT_H_ */
