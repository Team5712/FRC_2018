#include "driveType/CompChassis.h"
#include <iostream>
/**
 * Initalize the objects needed for this Chassis type here. Due note that
 * these operations are called in RobotInit(), and will take effect as
 * soon as the robot is powered on. For exmaple, a timer started in
 * the constructor will continue to run and may return unexpected
 * or incorrect values. Instead, start the timer in the designated
 * init() method (autonomousInit() or teleopInit()).
 */
CompChassis::CompChassis() {

	l_master = new WPI_TalonSRX(1);
	r_master = new WPI_TalonSRX(5);

	l_slave1 = new WPI_VictorSPX(2);
	l_slave2 = new WPI_VictorSPX(3);
	l_slave3 = new WPI_VictorSPX(4);

	r_slave1 = new WPI_VictorSPX(6);
	r_slave2 = new WPI_VictorSPX(7);
	r_slave3 = new WPI_VictorSPX(8);

	l_intake = new WPI_VictorSPX(9);
	r_intake = new WPI_VictorSPX(10);

	lift_master = new WPI_TalonSRX(11);

	drive = new DifferentialDrive(*l_master, *r_master);

	pot = new AnalogPotentiometer(0, 1, 0);

	gyro = new AHRS(SPI::Port::kMXP, AHRS::kRawData, 200 /*samples/sec*/);

	joystick_l = new Joystick(0);
	joystick_r = new Joystick(1);
	joystick_lift = new Joystick(2);

	// Define the slaves and configure the master to use a QuadEncoder
	// TOOD: Make sure GetBaseID() returns the Id's for the correct master
	l_master->SetInverted(true);
	l_slave1->Set(ControlMode::Follower, l_master->GetBaseID());
	l_slave2->Set(ControlMode::Follower, l_master->GetBaseID());
	l_slave3->Set(ControlMode::Follower, l_master->GetBaseID());
	l_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);


	r_slave1->Set(ControlMode::Follower, r_master->GetBaseID());
	r_slave2->Set(ControlMode::Follower, r_master->GetBaseID());
	r_slave3->Set(ControlMode::Follower, r_master->GetBaseID());
	r_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

	for(int i = 0; i < 100 ; i++) {
		led_values[i] = 0.99 - (i * 0.02);
	}

//	l_intake->Set();
//	r_intake->Set();

}

/**
 * Delete and dereference pointers here. Avoid deleting pointers
 * that originate from BaseDrive.hpp as it will delete the pointers in its
 * deconstructor.
 */
CompChassis::~CompChassis() {
	// De-allocate pointers
	delete l_master;
	delete r_master;

	delete joystick_l;
	delete joystick_r;
	delete joystick_lift;
	delete lift_master;
	delete pot;

	delete l_slave1;
	delete l_slave2;
	delete l_slave3;

	delete r_slave1;
	delete r_slave2;
	delete r_slave3;

	delete r_intake;
	delete l_intake;
}

// ---- ROBOT.CPP METHODS ----

///**
// * Called by Robot.cpp when the Autonomous mode was initalized. Operations
// * may include resetting the value of encoders, starting timers, or zeroing
// * in the gyro. Overrides from BaseDrive.hpp
// */
void CompChassis::autonomousInit() {
	l_master->SetSelectedSensorPosition(0, 0, 10);
	r_master->SetSelectedSensorPosition(0, 0, 10);

}

/**
 * Called by Robot.cpp during autonomous. This is a looped function and will
 * be caused every cycle of the Robot. This should include all of the
 * operations for autonomous. Overrides from BaseDrive.hpp
 */
void CompChassis::autonomousPeriodic() {
	char str[80];
	sprintf(str, "L encoder = %d", l_master->GetSelectedSensorPosition(0));
	DriverStation::ReportError(str);

	r_master->Set(0.7);
}

/**
 * Method that is called at the beginning of TeleOp. It will be called once
 * and should initalize and set the defaults for the Robot. Overrides
 * from BaseDrive.hpp
 */
void CompChassis::teleopInit() {
	l_master->SetSelectedSensorPosition(0, 0, 10);
	r_master->SetSelectedSensorPosition(0, 0, 10);

	char str[80];
	sprintf(str, "starting comp chassis");
	DriverStation::ReportError(str);
}

void CompChassis::handleInput() {

	// raise to top/scale
	if (joystick_lift->GetRawButtonPressed(4)) {
		lift_isSwitch = false;
		lift_isFloor = false;
		lift_isScale = !lift_isScale;
	}
	// move to switch
	if (joystick_lift->GetRawButtonPressed(2)) {
		lift_isScale = false;
		lift_isFloor = false;
		lift_isSwitch = !lift_isSwitch;
	}
	// lower to floor
	if (joystick_lift->GetRawButtonPressed(1)) {
		lift_isSwitch = false;
		lift_isScale = false;
		lift_isFloor = !lift_isFloor;
	}
}

/**
 * Called in a loop from Robot.cpp. It should include general operations for
 * driving the robot, such as joystick input, motor output, and reading
 * various sensor values. Overrides from BaseDrive.hpp
 */
void CompChassis::teleopPeriodic() {


	handleInput();

	// the higher the value, the lower it is
	if (lift_isFloor) {

		current_led = 0.77;

		// dont need to check if it's greater because the lift is already at the floor
		if (pot->Get() > lift_potFloor) {
			//std::cout << "floor " << pot->Get() << std::endl;
			lift_master->Set(-0.25);
		} else {
			//std::cout << "stop" << std::endl;
			lift_master->Set(0.0);
		}

	} else if (lift_isSwitch) {

		current_led = 0.61;

		// if were within a certain range
		if (pot->Get() < lift_potSwitch) {
			//std::cout << "stop" << std::endl;
			lift_master->Set(0.0);
			lift_isSwitch = false;
		} else {
			//std::cout << "switch " << pot->Get() << std::endl;
			lift_master->Set(0.70);
		}

	} else if (lift_isScale)
	{

		current_led = 0.69;

		if (pot->Get() < lift_potScale) {
			//std::cout << "stop" << std::endl;
			lift_master->Set(0.0);
			lift_isScale = false;
		} else {
			//std::cout << "scale " << pot->Get() << std::endl;
			lift_master->Set(0.50);
		}
	}

	//std::cout << pot->Get() << std::endl;

	if (joystick_lift->GetRawButtonPressed(5)) {
		led_current -= 1;
		//std::cout << led_values[led_current] << std::endl;
	}
	// left bumper switch led
	if (joystick_lift->GetRawButtonPressed(6)) {
		led_current += 1;
		//std::cout << led_values[led_current] << std::endl;
	}

	led->Set(led_values[led_current]);
	//led->Set(current_led);

	drive->TankDrive(joystick_l->GetRawAxis(1), joystick_r->GetRawAxis(1), true);
	//drive->TankDrive(0, 0, true);

	// ignore super small accidental bumps on the right joy stick; down is positive on joystick
//	if(abs(joystick_lift->GetRawAxis(5)) > 0.025) {
//		lift_isScale = false;
//		lift_isSwitch = false;
//		lift_isFloor = false;
//		//lift_master->Set(joystick_lift->GetRawAxis(5));
////		std::cout << joystick_lift->GetRawAxis(5) << std::endl;
//	}
}

// ---- END ROBOT.CPP METHODS ----
/**
 * Arcade drive method for differential drive platform. The calculated values
 * will be squared to decrease sensitivity at low speeds. This method
 * will use the motors and configurations required by the Competition Chassis.
 *
 * @param xSpeed
 * the robot's speed along the X axis [-1.0..1.0]. Forward is positive.
 * @param zRotation
 * the robot's rotation rate around the Z axis [-1.0..1.0]. Clockwise is positive.
 * @param squaredInputs
 * default value is true, which will help limit sensitivity at slow speeds.
 */
void CompChassis::ArcadeDrive(double xSpeed, double zRotation,
		bool squaredInputs) {
	drive->ArcadeDrive(xSpeed, zRotation, squaredInputs);
}

/**
 * The rotation argument controls the curvature of the robot's path rather
 * than its rate of heading change. This makes the robot more controllable
 * at high speeds. Also handles the robot's quick turn functionality
 * - "quick turn" overrides constant-curvature turning for turn-in-place
 * maneuvers. This will use the configuration for the Competition Chassis.
 *
 * The main difference between this method and ArcadeDrive is that the
 * zRotation equals the heading of the robot in this method; in ArcadeDrive,
 * zRotation specifies how much the robot will turn.
 *
 * @param xSpeed
 * the robot's speed along the X axis [-1.0..1.0]. Forward is positive.
 * @param zRotation
 * the robot's heading around the Z axis [-1.0..1.0]. Clockwise is positive.
 * @param isQuickTurn
 * if set, overrides constant-curvature turning for turn-in-place maneuvers.
 *
 */
void CompChassis::CurvatureDrive(double xSpeed, double zRotation,
		bool isQuickTurn) {
	drive->CurvatureDrive(xSpeed, zRotation, isQuickTurn);
}

/**
 * Drive that explicitly takes the power of the left and right sides of the
 * robot. squaredInputs will help reduce sensitivity if set to true. This
 * method will use the configuration for the Competition Chassis.
 *
 * @param leftSpeed
 * the speed of the left side of the robot
 * @param rightSpeed
 * the speed of the right side of the robot
 * @param squredInputs
 * defalut value is true. This will control how sensitive the robot is
 */
void CompChassis::TankDrive(double leftSpeed, double rightSpeed,
		bool squaredInputs) {
	drive->TankDrive(leftSpeed, rightSpeed, squaredInputs);
}

/**
 * Attempt to drive in a straight line using the pre-defined bias variables.
 * This method could be used to conduct an automated test of the robot to help
 * determine the ideal bias values for a completely straight path.
 *
 * @param speed
 * the speed that the robot should drive forward at
 */
void CompChassis::driveStraight(double speed) {
	TankDrive(speed * Constants::bias_ratio + Constants::bias_offset,
			speed * Constants::bias_ratio + Constants::bias_offset);
}

/**
 * This will return the encoder ticks for the left and right master motors. It will
 * return as an integer pointer, but should be used an array. The first index [0]
 * is the left encoder tick count, and the second index [1] is the right
 * encoder tick count;
 *
 * @return
 * An integer pointer that contains both encoder values.
 */
int* CompChassis::getEncoderValues() {
	return new int[l_master->GetSelectedSensorPosition(0), r_master->GetSelectedSensorPosition(0)];
}

/**
 * Returns a float representing the yaw (or angle) returned by the Gyro on
 * the robot.
 *
 * @return
 * A float from -180 to +180 representing the degree that the robot is facing.
 */
float CompChassis::getGyroYaw() {
	return gyro->GetYaw();
}

/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

