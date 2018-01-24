#include "Robot.h"
#include <iostream>

#define IS_COMPETITION false

Robot::Robot()
{

	#if IS_COMPETITION
		drive = new CompChassis();
	#else
		drive = new PeanutChassis();
	#endif


}

Robot::~Robot()
{

}

void Robot::RobotInit()
{

}

void Robot::AutonomousInit()
{
	drive->autonomousInit();
}

void Robot::AutonomousPeriodic()
{
	drive->autonomousPeriodic();
}

void Robot::TeleopInit()
{
	drive->teleopInit();
}

void Robot::TeleopPeriodic()
{
	drive->teleopPeriodic();
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

