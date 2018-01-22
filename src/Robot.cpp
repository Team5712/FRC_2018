#include "Robot.h"
#include <iostream>

#define IS_COMPETITION true

Robot::Robot() {

}

Robot::~Robot() {

}

/*
 * 		char str[80];
 sprintf(str, "Connected to the Jetson!");
 DriverStation::ReportError(str);
 *
 *  */

void Robot::RobotInit() {

#ifdef IS_COMPETITION

	robot = new CompetitionChassis();

#else

	robot = new PeanutChassis();

#endif



}

void Robot::AutonomousInit() {

}

void Robot::AutonomousPeriodic() {
	robot->teleopPeriodic();
}

void Robot::TeleopInit() {
}

void Robot::TeleopPeriodic() {

#ifdef IS_COMPETITION

#else

#endif



}

START_ROBOT_CLASS(Robot)

// OLD STRUCTURE
//class Robot : public frc::IterativeRobot {
//public:
//	void RobotInit() {
//
//	}
//
//	/*
//	 * This autonomous (along with the chooser code above) shows how to
//	 * select
//	 * between different autonomous modes using the dashboard. The sendable
//	 * chooser code works with the Java SmartDashboard. If you prefer the
//	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
//	 * GetString line to get the auto name from the text box below the Gyro.
//	 *
//	 * You can add additional auto modes by adding additional comparisons to
//	 * the
//	 * if-else structure below with additional strings. If using the
//	 * SendableChooser make sure to add them to the chooser code above as
//	 * well.
//	 */
//	void AutonomousInit() override {
//
//	}
//
//	void AutonomousPeriodic() {
//
//	}
//
//	void TeleopInit() {}
//
//	void TeleopPeriodic() {}
//
//	void TestPeriodic()
//	{}
//
//private:
//
//};

