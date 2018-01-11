#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

// Order imports based on most global -> local

#include "WPILib.h"

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

};



#endif /* SRC_ROBOT_H_ */
