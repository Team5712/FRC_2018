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
CompChassis::CompChassis(Spark *l) {

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
	// 3, 4 for comp

	cylinderClamp = new Solenoid(6);
	cylinderLift = new Solenoid(7);

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

	r_slave1->Follow(*r_master);
	r_slave2->Follow(*r_master);
	r_slave3->Follow(*r_master);

//	initMotionMagic();

	lift_master->ConfigSelectedFeedbackSensor(FeedbackDevice::Analog, 0, 10);
	led = l;
	for (int i = 0; i < 100; i++) {
		led_values[i] = 0.99 - (i * 0.02);
	}
	lift_master->SetNeutralMode(NeutralMode::Brake);

	limitSwitch = new DigitalInput(9);
	// disengage
	climber->Set(DoubleSolenoid::Value::kReverse);

	current_led = -0.99;
//
//	l_master->ConfigClosedloopRamp(0.75,0);
//	l_slave1->ConfigClosedloopRamp(0,0);
//	l_slave2->ConfigClosedloopRamp(0,0);
//	l_slave3->ConfigClosedloopRamp(0,0);
//	r_master->ConfigClosedloopRamp(0.75,0);
//	r_slave1->ConfigClosedloopRamp(0,0);
//	r_slave2->ConfigClosedloopRamp(0,0);
//	r_slave3->ConfigClosedloopRamp(0,0);

//	table = NetworkTable::GetTable("limelight");
//	camera = CameraServer::GetInstance()->StartAutomaticCapture(0);
//	camera.SetResolution(320, 240);

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

void CompChassis::initMotionMagic() {

	l_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,
			0.0001);
	r_master->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0,
			0.0001);

	l_master->SelectProfileSlot(0, 0);
	r_master->SelectProfileSlot(0, 0);

	l_master->Config_kP(0, mm.l_P, mm.TIMEOUT);
	l_master->Config_kI(0, mm.l_I, mm.TIMEOUT);
	l_master->Config_kD(0, mm.l_D, mm.TIMEOUT);
	l_master->Config_kF(0, mm.l_F, mm.TIMEOUT);

	r_master->Config_kP(0, mm.r_P, mm.TIMEOUT);
	r_master->Config_kI(0, mm.r_I, mm.TIMEOUT);
	r_master->Config_kD(0, mm.r_D, mm.TIMEOUT);
	r_master->Config_kF(0, mm.r_F, mm.TIMEOUT);

	l_master->ConfigMotionAcceleration(mm.ACCEL, mm.TIMEOUT);
	l_master->ConfigMotionCruiseVelocity(mm.VEL, mm.TIMEOUT);

	r_master->ConfigMotionAcceleration(mm.ACCEL, mm.TIMEOUT);
	r_master->ConfigMotionCruiseVelocity(mm.VEL, mm.TIMEOUT);

	l_master->SetSelectedSensorPosition(0, 0, mm.TIMEOUT);
	r_master->SetSelectedSensorPosition(0, 0, mm.TIMEOUT);

	l_master->SetSensorPhase(true);
}

bool CompChassis::isAtPIDPosition(double distance) {
	return (abs(
			l_master->GetSelectedSensorPosition(0)
					- (distance * mm.TICKS_PER_FOOT)) < 500);
}

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

//	std::cout << "left: " << getLeftValue() << std::endl;
//	std::cout << "right: " << getRightValue() << std::endl;
//	std::cout << "POT " << lift_master->GetSelectedSensorPosition(0) << std::endl;

	// if were not climbing drive normally
	if (!isClimbing) {

		TankDrive(-(l_joystick->GetRawAxis(1) * r_ratio) + left_command,
				r_joystick->GetRawAxis(1) + right_command, true);
		// we can only drive forward with pto activated
	} else {
		TankDrive(-abs(l_joystick->GetRawAxis(1)),
				-abs(r_joystick->GetRawAxis(1)) + right_command, true);
	}


	//drive->TankDrive(0, 0, true);
}

void CompChassis::handleInput() {

	if (joystick_lift->GetRawButton(3)) {
		isX = true;
	} else {
		isX = false;
	}

	if (joystick_lift->GetRawButtonPressed(10)) {
		isRightStickButton = true;
	} else {
		isRightStickButton = false;
	}

	if (l_joystick->GetRawButton(3)) {
		isLeftMiddleButton = true;
	} else {
		isLeftMiddleButton = false;
	}

	if (r_joystick->GetRawButton(2)) {
		isRightMiddleButton = true;
	} else {
		isRightMiddleButton = false;
	}

	// right buttons
	if (joystick_lift->GetRawButton(1)) {
		isA = true;
	} else {
		isA = false;
	}
	if (joystick_lift->GetRawButton(2)) {
		isB = true;
	} else {
		isB = false;
	}
	if (joystick_lift->GetRawButton(4)) {
		isY = true;
	} else {
		isY = false;
	}

	if (joystick_lift->GetRawButton(5)) {
		isLeftBumper = true;
	} else {
		isLeftBumper = false;
	}
	//press again to shift back
	if (joystick_lift->GetRawButton(6)) {
		isRightBumper = true;
	} else {
		isRightBumper = false;
	}

	leftJoystickAxis = joystick_lift->GetRawAxis(2);
	rightJoystickAxis = joystick_lift->GetRawAxis(3);

	if (leftJoystickAxis > 0.95) {
		isLeftTrigger = true;
	} else {
		isLeftTrigger = false;
	}

	if (joystick_lift->GetRawButton(8)) {
		isStartButton = true;
	} else {
		isStartButton = false;
	}

	if (rightJoystickAxis > 0.95) {
		isRightTrigger = true;
	} else {
		isRightTrigger = false;
	}

	if (l_joystick->GetRawButton(1)) {
		isLeftDriveTrigger = true;
	} else {
		isLeftDriveTrigger = false;
	}

	if (r_joystick->GetRawButton(1)) {
		isRightDriveTrigger = true;
	} else {
		isRightDriveTrigger = false;
	}

}

bool CompChassis::succCube(double power) {

	if (!isSucc_timer_start) {
		succ_timer.Start();
		isSucc_timer_start = true;
	}

	if (succ_timer.Get() < succ_cube_duration) {

		cylinderClamp->Set(DoubleSolenoid::Value::kReverse);
		cylinderLift->Set(true);

		if (!isGrip_timerStart) {
			grip_timer.Start();
			isGrip_timerStart = true;
		}

		// left
		if (grip_timer.Get() < pulse_interval && grip_isLeft) {
			l_grip->Set(onPulsePower);
			// this is the right side
		} else if (grip_timer.Get() < pulse_interval) {
			l_grip->Set(offPulsePower);
			r_grip->Set(onPulsePower);
		} else {
			// we are done with a cycle on one side of the wheels
			isGrip_timerStart = false;
			grip_isLeft = !grip_isLeft;
			grip_timer.Reset();
		}

		return false;
	} else {
		cylinderClamp->Set(DoubleSolenoid::Value::kForward);
		cylinderLift->Set(false);

		l_grip->Set(0.0);
		r_grip->Set(0.0);
		isSucc_timer_start = false;
		succ_timer.Reset();
		return true;
	}

	return false;
}

bool CompChassis::shootCube(double power) {

	// TODO negative grip power shoots the cube
	if (!isShoot_timer_start) {
		shoot_timer.Start();
		isShoot_timer_start = true;
	}

	if (shoot_timer.Get() < shoot_cube_duration) {
		l_grip->Set(power);
		r_grip->Set(power);
		return false;
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

	// alternating intakes left bumper or the drivers left joystick middle button if we dont have a cube in the intake already
	if (isLeftBumper || isLeftMiddleButton || isRightMiddleButton) {

		cylinderLift->Set(true);
		cylinderClamp->Set(true);

		if (!isGrip_timerStart) {
			grip_timer.Start();
			isGrip_timerStart = true;
		}

		// left
		if (grip_timer.Get() < pulse_interval && grip_isLeft) {
			l_grip->Set(onPulsePower);
			// this is the right side
		} else if (grip_timer.Get() < pulse_interval) {
			l_grip->Set(offPulsePower);
			r_grip->Set(onPulsePower);
		} else {
			// we are done with a cycle on one side of the wheels
			isGrip_timerStart = false;
			grip_isLeft = !grip_isLeft;
			grip_timer.Reset();
		}

		// none of the other statements will execute because its being held down, once it's released it will continue to the else
		isDoneIntaking = true;
		intakeDelayTimer.Reset();

		// right bumper spit out cube
	} else if (isRightBumper) {

		isDoneIntaking = false;
		// if were at the scale, shoot it out slower
		if (lift_master->GetSelectedSensorPosition(0) < 100) {
			l_grip->Set(-0.35);
			r_grip->Set(-0.35);
		} else {
			l_grip->Set(-0.50);
			r_grip->Set(-0.50);
		}

		// if none of the intake buttons are pressed
	} else {

		l_grip->Set(0.35);
		r_grip->Set(0.35);

		// if we're done with the left bumper
		if (isDoneIntaking) {
			if (!isIntakeDelayTimerStart) {
				intakeDelayTimer.Start();
				isIntakeDelayTimerStart = true;
			}

			cylinderClamp->Set(false);

			// clamp down on the cube slightly before the pneumatic lift goes up
			if (intakeDelayTimer.Get() > intakeDelay) {
				isIntakeDelayTimerStart = false;
				intakeDelayTimer.Reset();
				cylinderLift->Set(false);
				isDoneIntaking = false;
			}

		} else {
			cylinderLift->Set(false);

			if(isX || isLeftBumper) {
				cylinderClamp->Set(true);
			} else {
				cylinderClamp->Set(false);
			}
		}
	}

	// disable intake as we near the scale
	if (lift_master->GetSelectedSensorPosition(0) < lift_potSwitch - 40
			&& !isRightBumper) {
		l_grip->Set(0.0);
		r_grip->Set(0.0);
	}

	if (isA) {
		cylinderLift->Set(true);
	}

	if (isB) {
		l_grip->Set(0.65);
		r_grip->Set(0.65);
	}

	if (isA) {
		cylinderLift->Set(true);
	}

	if (isX) {
		cylinderClamp->Set(true);
	}

	if (isY) {
		l_grip->Set(-1.0);
		r_grip->Set(-1.0);
	}
}

void CompChassis::updateShifter() {
	//hold both triggers on drive joysticks to shift into high gear
	if (l_joystick->GetRawButton(1) || r_joystick->GetRawButton(1)) {
		compressor->Stop();
		shifter->Set(false);
	} else if (!isClimbing) {
		compressor->Start();
		shifter->Set(true);
	}
}

void CompChassis::updateClimber() {

	// enable the pto shaft, set into low gear
	if (joystick_lift->GetRawAxis(2) > 0.90) {
		isClimbing = true;
		climber->Set(DoubleSolenoid::Value::kForward);
		std::cout << "is now climbing!" << std::endl;
	}
	// left trigger disables pto shaft, will remain in low gear for reamainder of match
	if (joystick_lift->GetRawAxis(3) > 0.95) {
		std::cout << "cancel climb" << std::endl;
		climber->Set(DoubleSolenoid::Value::kReverse);
		isClimbing = false;
	}

	// TODO true is low gear
	if (!isClimbing && !l_joystick->GetRawButton(1)
			&& !r_joystick->GetRawButton(1)) {
		shifter->Set(true);
		compressor->Start();
	}
	if (isClimbing) {
		compressor->Stop();
		shifter->Set(false);
	}
}

void CompChassis::updatePneumatics() {

}

void CompChassis::updateBlinkin() {

	// -0.25 red breath

	// if scale position lift
	if (lift_master->GetSelectedSensorPosition(0) < lift_potScale + 70) {
		current_led = 0.07;

		// if switch position
	} else if (lift_master->GetSelectedSensorPosition(0) < lift_potFloor + 20
			&& lift_master->GetSelectedSensorPosition(0)
					> lift_potSwitch - 80) {
		current_led = -0.25;
	} else if(lift_master->GetSelectedSensorPosition(0) < lift_potScale + 20) {
		current_led = -0.99;
	} else {
		current_led = 0.53;
	}

	if (isClimbing) {
		current_led = -0.99;
	}

	led->Set(current_led);
}

void CompChassis::updateLift() {

//	std::cout << lift_master->GetSelectedSensorPosition(0) << std::endl;

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
			lift_master->Set(liftSustainPower);
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
			lift_master->Set(liftSustainPower);
			lift_isScale = false;
		} else {
			l_grip->Set(0.70);
			r_grip->Set(0.70);
			lift_master->Set(1.00);
		}
	}

//	// if we are above a certain position, shift into low to prevent falling over
//	if (lift_master->GetSelectedSensorPosition(0) < 280) {
//		shifter->Set(true);
//		// set it back to high if the drive triggers aren't pressed
//	} else if (!isLeftDriveTrigger && !isRightDriveTrigger) {
//		shifter->Set(false);
//	}

	// if the input is greater than a small amount
	if (getLiftJoystick() > 0.13 || getLiftJoystick() < -0.25) {

		lift_isFloor = false;
		lift_isScale = false;
		lift_isSwitch = false;

		// joystick going down
		if (getLiftJoystick() < 0) {

			// if we go lower than the floor height, set to zero so we dont smash into robot, stop the lift proportionately to how fast we're going
			if (lift_master->GetSelectedSensorPosition(0) > lift_potFloor) {
				lift_master->Set(0.0);
			} else {
				// scale down the power slightly
				lift_master->Set(getLiftJoystick() * 0.8);
			}

			// joystick going up
		} else {
			// scale lift height proportionately to how fast we're running the lift
			if (lift_master->GetSelectedSensorPosition(0)
					< lift_potScale + abs(getLiftJoystick() * 10)) {
				lift_master->Set(liftSustainPower);
				std::cout << "above lift" << std::endl;
			} else {
				lift_master->Set(getLiftJoystick() * 1.1);
			}
		}
		// if we have no set point then just leave it at the same position
	} else if (!lift_isFloor && !lift_isScale && !lift_isSwitch) {
		lift_master->Set(liftSustainPower);
	}
}

bool CompChassis::liftFloor() {


// dont need to check if it's greater because the lift is already at the floor
	if (lift_master->GetSelectedSensorPosition(0) > lift_potFloor) {
		lift_master->Set(0.0);
		return true;
	} else {
		lift_master->Set(-0.65);
		return false;
	}
}

bool CompChassis::liftScale() {
	if (lift_master->GetSelectedSensorPosition(0) < lift_potScale) {
		lift_master->Set(liftSustainPower);
		return true;
	} else {
		lift_master->Set(1.0);
		return false;
	}
}

bool CompChassis::liftSwitch() {
	if (lift_master->GetSelectedSensorPosition(0) < lift_potSwitch) {
		lift_master->Set(liftSustainPower);
		return true;
	} else {
		lift_master->Set(0.75);
		return false;
	}
}

// alaex is a terd

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
	return ((l_master->GetOutputCurrent() + l_slave1->GetOutputCurrent()
			+ l_slave2->GetOutputCurrent() + l_slave3->GetOutputCurrent()) / 4);
}

double CompChassis::getCurrentRight() {
	return (r_master->GetOutputCurrent() + r_slave1->GetOutputCurrent()
			+ r_slave2->GetOutputCurrent() + r_slave3->GetOutputCurrent()) / 4;
}

/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

void CompChassis::resetEncoders() {
	r_master->SetSelectedSensorPosition(0, 0, 0);
	l_master->SetSelectedSensorPosition(0, 0, 0);
}

/**
 * positive goes towards the robot
 */
void CompChassis::setGrippers(double power) {
	l_grip->Set(power);
	r_grip->Set(power);
}

void CompChassis::updateVisionButtons() {
//
//	float tx = table->GetNumber("tx", 0);
//	float kp = -0.01;
//	float min_command = 0.05;
//
//	if (joystick_lift->GetRawButton(7)) {
//		float heading_error = -tx;
//		float steering_adjust = 0.0f;
//
//		if (tx > .5) {
//			steering_adjust = kp * heading_error - min_command;
//		} else if (tx < -.5) {
//			steering_adjust = kp * heading_error + min_command;
//		}
//
//		left_command -= steering_adjust;
//		right_command += steering_adjust;
//
//		std::cout << "tx " << tx << std::endl;
//		std::cout << "left " << left_command << std::endl;
//		std::cout << "right " << right_command << std::endl;
//		//setLeftRight(left_command, right_command);
//	} else if (!joystick_lift->GetRawButton(7)) {
//		left_command = 0;
//		right_command = 0;
//	}

}

void CompChassis::CurvatureDrive(double xSpeed, double zRotation,
		bool isQuickTurn) {
	drive->CurvatureDrive(xSpeed, zRotation, isQuickTurn);
}

void CompChassis::TankDrive(double leftSpeed, double rightSpeed,
		bool squaredInputs) {
	drive->TankDrive(leftSpeed, rightSpeed, squaredInputs);
}

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
	l_master->Set(v);
	r_master->Set(v2 * 1.0731);
}

void CompChassis::ArcadeDrive(double xSpeed, double zRotation,
		bool squaredInputs) {
	drive->ArcadeDrive(xSpeed, zRotation, squaredInputs);
}

void CompChassis::raiseIntake() {
	cylinderClamp->Set(false);
	cylinderLift->Set(false);
}

void CompChassis::lowerIntake() {
	cylinderClamp->Set(true);
	cylinderLift->Set(true);
	l_grip->Set(0.5);
	r_grip->Set(0.5);
}

void CompChassis::setLed(double value) {
	current_led = value;
	led->Set(value);
}

/**
 * get the real value of the joystick, so up will go up and down will go down
 */
double CompChassis::getLiftJoystick() {
	return joystick_lift->GetRawAxis(1) * -1;
}
