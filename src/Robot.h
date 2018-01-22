#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

// Order imports based on most global -> local

#include "WPILib.h"
#include <iostream>
#include <string>
#include <cmath>

#include <IterativeRobot.h>
#include "ctre/Phoenix.h"
#include "Drive/DifferentialDrive.h"
#include "./driveType/CompetitionChassis.h"
#include "./driveType/PeanutChassis.h"
#include "./driveType/BaseDrive.h"
#include "Joystick.h"


class Robot : public frc::IterativeRobot
{
public:
	Robot();
	~Robot();

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
	BaseDrive *robot;
	DifferentialDrive * diffDrive;
	Joystick *joystick;
};



#endif /* SRC_ROBOT_H_ */
