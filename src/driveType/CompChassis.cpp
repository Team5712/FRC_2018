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

	lift_master = new WPI_TalonSRX(11);

	l_grip = new WPI_VictorSPX(12);
	r_grip = new WPI_VictorSPX(13);

	drive = new DifferentialDrive(*l_master, *r_master);

	pot = new AnalogPotentiometer(0, 1, 0);

	compressor = new Compressor(0);

	shifter = new Solenoid(2);
	climber = new DoubleSolenoid(3, 4); //temp portss

	cylinder = new DoubleSolenoid(0, 7);

	gyro = new AHRS(SPI::Port::kMXP, AHRS::kRawData, 200 /*samples/sec*/);
	gyro->Reset();
	gyro->ZeroYaw();

	l_joystick = new Joystick(0);
	r_joystick = new Joystick(1);
	joystick_lift = new Joystick(2);

	// Define the slaves and configure the master to use a QuadEncoder
	// TOOD: Make sure GetBaseID() returns the Id's for the correct master
	l_slave1->Follow(*l_master);
	l_slave2->Follow(*l_master);
	l_slave3->Follow(*l_master);

	l_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

	r_slave1->Follow(*r_master);
	r_slave2->Follow(*r_master);
	r_slave3->Follow(*r_master);
	r_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	lift_master->ConfigSelectedFeedbackSensor(FeedbackDevice::Analog, 0, 10);
	led = new Spark(9);

	for (int i = 0; i < 100; i++) {
		led_values[i] = 0.99 - (i * 0.02);
	}
	lift_master->SetNeutralMode(NeutralMode::Brake);

	limitSwitch = new DigitalInput(9);

	// disengage
	climber->Set(DoubleSolenoid::Value::kReverse);

	current_led = 0.31;

	table = NetworkTable::GetTable("limelight");
	camera = CameraServer::GetInstance()->StartAutomaticCapture(0);
	camera.SetResolution(320, 240);
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

	delete l_joystick;
	delete r_joystick;
	delete joystick_lift;
	delete lift_master;
	delete pot;

	delete l_slave1;
	delete l_slave2;
	delete l_slave3;

	delete r_slave1;
	delete r_slave2;
	delete r_slave3;
}

// ---- ROBOT.CPP METHODS ----

///**
// * Called by Robot.cpp when the Autonomous mode was initalized. Operations
// * may include resetting the value of encoders, starting timers, or zeroing
// * in the gyro. Overrides from BaseDrive.hpp
// */
void CompChassis::autonomousInit() {

	match_timer.Start();

	l_master->SetSelectedSensorPosition(0, 0, 10);
	r_master->SetSelectedSensorPosition(0, 0, 10);
	gyro->ZeroYaw();

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

}

/**
 * Method that is called at the beginning of TeleOp. It will be called once
 * and should initalize and set the defaults for the Robot. Overrides
 * from BaseDrive.hpp
 */
void CompChassis::teleopInit() {
	// set encoders to 0
	l_master->SetSelectedSensorPosition(0, 0, 10);
	r_master->SetSelectedSensorPosition(0, 0, 10);

	// disengage
	climber->Set(DoubleSolenoid::Value::kReverse);

	char str[80];
	sprintf(str, "teleop init...");
	DriverStation::ReportError(str);
	gyro->ZeroYaw();
}


//2350
/** TODO:
 * Called in a loop from Robot.cpp. It should include general operations for
 * driving the robot, such as joystick input, motor output, and reading
 * various sensor values. Overrides from BaseDrive.hpp
 */
void CompChassis::teleopPeriodic() {
	compressor->Start();
	handleInput();
	updateVisionButtons();
	updateLift();
	updateBlinkin();
	updateShifter();
	updatePneumatics();
	updateClimber();
	updateIntake();

	// if were not climbing drive normally
	if (!isClimbing) {

//		if(getCurrentLeft() > 200) || getCurrentRight() > 200 {
//
//		}

		TankDrive((l_joystick->GetRawAxis(1) * r_ratio) + left_command,
				r_joystick->GetRawAxis(1) + right_command, true);
	// we can only drive forward with pto activated
	} else {
		TankDrive(abs(l_joystick->GetRawAxis(1) * r_ratio) + left_command,
				abs(r_joystick->GetRawAxis(1)) + right_command, true);
	}
	//drive->TankDrive(0, 0, true);
}

void CompChassis::handleInput() {

	//press once to shift forward
	if (l_joystick->GetRawButtonPressed(4) && forward == true) {
		climber->Set(DoubleSolenoid::Value::kForward);

		forward = false;
	}
	//press again to shift back
	if (r_joystick->GetRawButtonPressed(4) && forward == false) {
		climber->Set(DoubleSolenoid::Value::kReverse);
		forward = true;
	}

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

bool CompChassis::shootCube() {

	if (!isShoot_timer_start) {
		shoot_timer.Start();
		isShoot_timer_start = true;
	}

	if (shoot_timer.Get() < shoot_cube_duration) {
		l_grip->Set(-0.4);
		r_grip->Set(-0.4);
	} else {
		l_grip->Set(0.0);
		r_grip->Set(0.0);
		isShoot_timer_start = false;
		shoot_timer.Reset();
		return true;
	}

	return false;
}

void CompChassis::updateIntake() {

	// alternating intakes left bumper if we dont have a cube in the intake already
	if (joystick_lift->GetRawButton(5) /*&& !limitSwitch->Get()*/) {

		cylinder->Set(DoubleSolenoid::Value::kReverse);

		if (!isGrip_timerStart) {
			grip_timer.Start();
			isGrip_timerStart = true;
		}

		// left
		if (grip_timer.Get() < pulse_interval && grip_isLeft) {
			l_grip->Set(1.0);
			r_grip->Set(0.20);
			// this is the right side
		} else if (grip_timer.Get() < pulse_interval) {
			l_grip->Set(0.20);
			r_grip->Set(1.0);
		} else {
			// we are done with a cycle on one side of the wheels
			std::cout << "reset " << std::endl;
			isGrip_timerStart = false;
			grip_isLeft = !grip_isLeft;
			grip_timer.Reset();
		}

		// right bumper spit out cube
	} else if (joystick_lift->GetRawButton(6)) {

		l_grip->Set(-0.50);
		r_grip->Set(-0.50);
		// if none of the intake buttons are pressed
	} else if (joystick_lift->GetRawButton(3)) {
		cylinder->Set(DoubleSolenoid::Value::kReverse);
	} else {
		// dega
		l_grip->Set(0.0);
		r_grip->Set(0.0);
		cylinder->Set(DoubleSolenoid::Value::kForward);
	}

	if(joystick_lift->GetRawButton(8)) {
		l_grip->Set(0.8);
		r_grip->Set(0.8);
	}
}

void CompChassis::updateShifter() {
	//hold both triggers on drive joysticks to shift into high gear
	if (l_joystick->GetRawButton(1) && r_joystick->GetRawButton(1)) {
		shifter->Set(true);
	} else {
		shifter->Set(false);
	}
}

void CompChassis::updateClimber() {

	if (joystick_lift->GetRawButtonPressed(9)) {
		isClimbing = true;
		shifter->Set(true);
		climber->Set(DoubleSolenoid::Value::kForward);
	}
	// left trigger disables pto shaft
	if(joystick_lift->GetRawAxis(2) > 0.95) {
		climber->Set(DoubleSolenoid::Value::kReverse);
		isClimbing = false;
	}
}

void CompChassis::updatePneumatics() {

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

void CompChassis::updateBlinkin() {

	if(isClimbing) {
		current_led = 0.15;
	}

	if(match_timer.Get() > 30 && match_timer.Get() < 60) {
		current_led = -0.63;
	}


	led->Set(current_led);
}

void CompChassis::updateVisionButtons() {

	float tx = table->GetNumber("tx", 0);
	float kp = -0.01;
	float min_command = 0.05;

	if (joystick_lift->GetRawButton(7)) {
		float heading_error = -tx;
		float steering_adjust = 0.0f;

		if (tx > .5) {
			steering_adjust = kp * heading_error - min_command;
		} else if (tx < -.5) {
			steering_adjust = kp * heading_error + min_command;
		}

		left_command -= steering_adjust;
		right_command += steering_adjust;

		std::cout << "tx " << tx << std::endl;
		std::cout << "left " << left_command << std::endl;
		std::cout << "right " << right_command << std::endl;
		//setLeftRight(left_command, right_command);
	} else if (!joystick_lift->GetRawButton(7)) {
		left_command = 0;
		right_command = 0;
	}

}

void CompChassis::updateLift() {

	// automatic button control
	if (lift_isFloor) {
		current_led = 0.77;
		// dont need to check if it's greater because the lift is already at the floor
		if (lift_master->GetSelectedSensorPosition(0) > lift_potFloor) {
			lift_master->Set(0.0);
			lift_isFloor = false;
		} else {
			l_grip->Set(0.70);
			r_grip->Set(0.70);
			lift_master->Set(-0.50);
		}
	}
	if (lift_isSwitch) {

		current_led = 0.61;
		if (lift_master->GetSelectedSensorPosition(0) < lift_potSwitch) {
			lift_master->Set(0.115);
			lift_isSwitch = false;
		} else {
			l_grip->Set(0.70);
			r_grip->Set(0.70);
			lift_master->Set(1.00);
		}
	}
	if (lift_isScale) {
		current_led = 0.69;
		if (lift_master->GetSelectedSensorPosition(0) < lift_potScale) {
			lift_master->Set(0.115);
			lift_isScale = false;
		} else {
			l_grip->Set(0.70);
			r_grip->Set(0.70);
			lift_master->Set(1.00);
		}
	}

	if(lift_master->GetSelectedSensorPosition(0) < 280) {
		shifter->Set(true);
	} else {
		shifter->Set(false);
	}
}

//std::cout << lift_master->GetSelectedSensorPosition(0) << std::endl;

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
	drive->TankDrive(-leftSpeed, rightSpeed, squaredInputs);
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
	setLeftRight(speed * Constants::bias_ratio + Constants::bias_offset,
			speed * Constants::bias_ratio + Constants::bias_offset);
}

int CompChassis::getLeftValue() {
	return l_master->GetSelectedSensorPosition(0);
}

int CompChassis::getRightValue() {
	return r_master->GetSelectedSensorPosition(0);
}

void CompChassis::setLeftRight(double v, double v2) {
	l_master->Set(v * 1.01);
	r_master->Set(v2);
}

bool CompChassis::liftFloor() {

	current_led = 0.77;

	// dont need to check if it's greater because the lift is already at the floor
	if (lift_master->GetSelectedSensorPosition(0) > lift_potFloor) {
		lift_master->Set(0.0);
		return true;
	} else {
		lift_master->Set(-0.45);
		return false;
	}
}

bool CompChassis::liftScale() {
	current_led = 0.69;
	if (lift_master->GetSelectedSensorPosition(0) < lift_potScale) {
		lift_master->Set(0.05);
		return true;
	} else {
		lift_master->Set(0.75);
		return false;
	}
}

bool CompChassis::liftSwitch() {
	current_led = 0.61;
	if (lift_master->GetSelectedSensorPosition(0) < lift_potSwitch) {
		lift_master->Set(0.05);
		return true;
	} else {
		lift_master->Set(0.75);
		return false;
	}
}

//class ExpFilter {
//public:
//	double percent;
//	double value;
//	ExpFilter(double percent = 0.8) {
//		this->percent = percent;
//		value = 0;
//	}
//
//	double filter(double nv) {
//		value = value * percent + nv * (1 - percent);
//		return value;
//	}
//
//	double getValue() { return value; }
//};

double CompChassis::getCurrentLeft() {
	//static ExpFilter filter;

	//return filter.filter((l_master->GetOutputCurrent() + l_slave1->GetOutputCurrent() + l_slave2->GetOutputCurrent() + l_slave3->GetOutputCurrent()) / 4);
	return 0.0;
}
	//return (r_master->GetOutputCurrent() + r_slave1->GetOutputCurrent() + r_slave2->GetOutputCurrent() + r_slave3->GetOutputCurrent()) / 4;


/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

void CompChassis::resetEncoders() {
	r_master->SetSelectedSensorPosition(0, 0, 0);
	l_master->SetSelectedSensorPosition(0, 0, 0);
}

void CompChassis::setGrippers(double power) {
	l_grip->Set(power);
	r_grip->Set(power);
}

