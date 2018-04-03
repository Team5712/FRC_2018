#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

// Order imports based on most local -> global

#include <autonomousModes/AutoBase.hpp>
#include "driveType/CompChassis.h"
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
#include "Priority.h"
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
	Priority priority = Priority::SCALE;

	// Basic methods
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	// Unneeded methods
	void DisabledInit() override;
	void DisabledPeriodic() override;
//	void TestInit();
//	void TestPeriodic();

private:
	AutoBase *autoMode;
	SDInterface sdinterface;
	CompChassis *drive;

	Timer led_timer;
	double led_update_interval = 3.0;
	bool isLedTimerStart = false;

	int led_current = 0;
	float *led_idle_values = new float[3];

	Spark *led;

};

#endif /* SRC_ROBOT_H_ */
