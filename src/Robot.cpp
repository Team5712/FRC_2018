#include "Robot.h"
// TODO check me
#define IS_COMPETITION true


Robot::Robot() {

	autoMode = 0;


	drive = new CompChassis();
	//drive = new PeanutChassis();
//#if IS_COMPETITION
//#else
//	drive = new PeanutChassis();
//#endif
	priority = sdinterface.getPriority();
	current_position = sdinterface.getStartingPosition();
}

Robot::~Robot() {

}

void Robot::RobotInit() {

}

void Robot::AutonomousInit() {
	drive->autonomousInit();

	// Stores a string of the switch and scale, i.e. "LRL", "LLR"
	// Directions are based on the direction of the team
	// (https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details)

	std::cout << "auto init..." << std::endl;


	// from the driver station
//	current_position = sdinterface.getStartingPosition();
	current_position = StartingPosition::MIDDLE;
	std::string game_data = DriverStation::GetInstance().GetGameSpecificMessage();
//	std::string game_data = "RRR";

	// Choose the autonomous mode based upon the game data [string: "LRL"] "Fitting it" - Matthew
	if (current_position == StartingPosition::LEFT) {
		autoMode = new AutoLeft(drive, game_data);

	} else if (current_position == StartingPosition::MIDDLE) {
		autoMode = new AutoMiddle(drive, game_data);

	} else if (current_position == StartingPosition::RIGHT) {
		autoMode = new AutoRight(drive, game_data);
	} else {
		autoMode = new AutoMiddle(drive, game_data);
	}

	autoMode->init();
}

void Robot::AutonomousPeriodic() {
	autoMode->run();
}

void Robot::TeleopInit() {
	drive->teleopInit();
}

void Robot::TeleopPeriodic() {
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

