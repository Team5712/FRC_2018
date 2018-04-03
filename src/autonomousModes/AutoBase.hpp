/*
 * AutoBase.hpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#ifndef SRC_AUTONOMOUSMODES_AUTOBASE_HPP_
#define SRC_AUTONOMOUSMODES_AUTOBASE_HPP_

#include "../driveType/CompChassis.h"
#include "../Constants.h"
#include <iostream>
#include "ctre/Phoenix.h"
#include "WPILib.h"

class AutoBase {
public:

	virtual ~AutoBase() {

	}

	double turn_power_slow = 0.25;
	double turn_slow_threshold = 0.15;

	/**
	 * drive at the specified motor output for a set amount of time and/or encoder value
	 */
	void timedDrive(double time, double left, double right, int e_left,
			int e_right) {

		Timer *timer = new Timer();
		int currentL, currentR = 0;

		// Take a "snapshot" of the encoder values instead of resetting them
		currentL = drive->getLeftValue();
		currentR = drive->getRightValue();

		timer->Reset();
		timer->Start();

		while ((abs(drive->getLeftValue() - currentL) < e_left)
				|| (abs(drive->getRightValue() - currentR) < e_right)
				|| (timer->Get() < time)) {
			drive->TankDrive(left, right);
		}
		// Stop the motors since the target / time was reached

		stop();
	}

	void timedDrive(double time, double left, double right) {

		Timer *timer = new Timer();

		timer->Reset();
		timer->Start();

		while (timer->Get() < time) {
			drive->TankDrive(left, right);
		}
		// When the loop ends, stop the motors
		stop();

	}

	void timedDrive(double left, double right, int e_left, int e_right) {

		int currentL, currentR = 0;

		// Take a "snapshot" of the encoder values instead of resetting them
		currentL = drive->getLeftValue();
		currentR = drive->getRightValue();

		while ((drive->getLeftValue() - abs(currentL) < e_left)
				&& (drive->getRightValue() - abs(currentR) < e_right)) {
			drive->TankDrive(
					left * Constants::bias_ratio + Constants::bias_offset,
					right * Constants::bias_ratio + Constants::bias_offset);
		}
		stop(); // Reached the target, so stop the motors

	}

	// stop the robot and rotate it at a given angle
	bool turn(double degrees, double power) {

		double yaw = drive->gyro->GetYaw();
		double distance = abs(degrees) - abs(yaw);

		if (degrees < 0) {

			// turn left
			if (yaw > degrees) {
				// if distance is less than 10% of the total distance
				if (distance > abs(degrees) * turn_slow_threshold) {
					drive->setLeftRight(-power, power);
				} else {
					drive->setLeftRight(-turn_power_slow, turn_power_slow);
				}

				return false;
			} else {
				std::cout << "[AutoBase.hpp][turn()] Finished!" << std::endl;
				stop();
				return true;
			}
		} else {
			if (yaw < degrees) {
//			std::cout << "[AutoBase.hpp][turn()] 1st Gyro Yaw:" << yaw << std::endl;
				drive->setLeftRight(power, -power);

				if (distance > degrees * turn_slow_threshold) {
					drive->setLeftRight(power, -power);
				} else {
					drive->setLeftRight(turn_power_slow, -turn_power_slow);
				}

				return false;
			} else {
				std::cout << "[AutoBase.hpp][turn()] Finished!" << std::endl;
				stop();
				return true;
			}
		}
	}

	// measured in 20 ms at a time
	double ramp_time = 0;
	// (ramp_time_goal / 20) * ramp_power will be how much you will ramp up
	double ramp_speed = 0.0075;
	double ramp_time_goal = 2000;
	// how much power we're giving the motors during ramp and deacceleration
	double ramp_power = 0;
	// minumum power that we add to pid_power
	double min_power = 0.1;

	// measured in 20 ms at a time
	double slow_time = 0;
	// (ramp_time_goal / 20) * ramp_power will be how much you will ramp up
	double slow_speed = 0.0075;
	// number of inches before our destination that we want to start slowing down
	double slow_time_start = 24;
	// how much power we're subtracting from the motors during ramp and deacceleration
	double slow_power = 0;
	// minumum power that we add to pid_power

	bool isSlowing = false;

	/*
	 *
	 * this will effectively "reset" the value of the encoder
	 *
	 * @param r_value
	 * same as l_value but on the right side
	 *
	 * @param inches
	 * the distance in inches to travel forward
	 * @param left
	 * the power of the left motors on the robot
	 * @param right
	 * the power of the right motors on the robot
	 *
	 * @returns
	 * 0 or 1
	 * 0: condition not satisfied
	 * 1: is at destination
	 */
	bool driveStraight(double inches, double power = 0.5) {

		ramp_time += 20;

//		std::cout << "left " << drive->getLeftValue() << std::endl;
//		std::cout << "right " << drive->getRightValue() << std::endl;
//		std::cout << "goal " << inches * Constants::RATIO << std::endl;

		double yaw = drive->gyro->GetYaw();
		double error = abs(yaw);
		double gyro_correction = 0.15;
		double gyro_range = 2;

		// if our encoders are greater than the goal minus how far we want to start slowing down, basically, we need to start slowing
		if (abs(drive->getLeftValue())
				> (Constants::RATIO * inches)
						- (slow_time_start * Constants::RATIO)
				|| abs(drive->getRightValue())
						> (Constants::RATIO * inches)
								- (slow_time_start * Constants::RATIO)) {
			std::cout << "encoderl " << abs(drive->getLeftValue()) << " goal: "
					<< (Constants::RATIO * inches)
							- (slow_time_start * Constants::RATIO) << std::endl;
			std::cout << "encoderr " << abs(drive->getRightValue()) << " goal: "
					<< (Constants::RATIO * inches)
							- (slow_time_start * Constants::RATIO) << std::endl;
			isSlowing = true;
		}


		if (isSlowing) {
			slow_power += slow_speed;
			std::cout << "slowing down " << ramp_power;
		}

		if (ramp_time < ramp_time_goal) {
			ramp_power += ramp_speed;
			std::cout << ramp_power << std::endl;
		} else {
			std::cout << "regular power now" << std::endl;
			ramp_power = power;
		}

//		std::cout << "correction: " << gyro_correction << std::endl;

		// if were drifting right
		if (yaw > gyro_range) {

			// if we are told to go forward
			if (inches > 0) {

				// if our current ticks are less than how far we need to go
				if (abs(drive->getLeftValue()) < Constants::RATIO * inches
						|| abs(drive->getRightValue())
								< Constants::RATIO * inches) {

					// if we're not ramping up and were ready to slow down
					if (isSlowing && ramp_time > ramp_time_goal) {
						drive->setLeftRight((power - slow_power) - min_power,
								(power - slow_power) - min_power
										+ gyro_correction);
					}

					// if we are still ramping up to our goal
					if (ramp_time < ramp_time_goal) {
						drive->setLeftRight(ramp_power + min_power,
								min_power + ramp_power + gyro_correction);
					} else {
						drive->setLeftRight(power, power + gyro_correction);
					}
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() < Constants::RATIO * inches
						|| drive->getRightValue() < Constants::RATIO * inches) {
					drive->setLeftRight(-power - gyro_correction, -power);
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			// if we're drifting left
		} else if (yaw < -gyro_range) {

			if (inches > 0) {
				if (abs(drive->getLeftValue()) < Constants::RATIO * inches
						|| abs(drive->getRightValue())
								< Constants::RATIO * inches) {

					// if we're not ramping up and were ready to slow down
					if (isSlowing && ramp_time > ramp_time_goal) {
						drive->setLeftRight(
								(power - slow_power) - min_power
										+ gyro_correction,
								(power - slow_power) - min_power);
					}

					// if we are still ramping up to our goal
					if (ramp_time < ramp_time_goal) {
						drive->setLeftRight(
								ramp_power + min_power + gyro_correction,
								min_power + ramp_power);
					} else {
						drive->setLeftRight(power + gyro_correction, power);
					}
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() < Constants::RATIO * inches
						|| drive->getRightValue() < Constants::RATIO * inches) {
					drive->setLeftRight(-power, -power - gyro_correction);
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			// if we're in the target gyro range
		} else {
			if (inches > 0) {
				if (abs(drive->getLeftValue()) < Constants::RATIO * inches
						|| abs(drive->getRightValue())
								< Constants::RATIO * inches) {

					// if we are still ramping up to our goal
					if (ramp_time < ramp_time_goal) {
						drive->setLeftRight(ramp_power + min_power,
								min_power + ramp_power);
					} else {
						drive->setLeftRight(power + gyro_correction, power);
					}

					drive->setLeftRight(power, power);
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() < Constants::RATIO * inches
						|| drive->getRightValue() < Constants::RATIO * inches) {
					drive->setLeftRight(-power, -power);
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
		}

		return false;
	}

//	bool runLiftUp(double tickGoal) {
//		if (lift_master->GetSelectedSensorPosition(0) < tickGoal) {
//			lift_master->Set(0.0);
//			return true;
//		} else {
//			lift_master->Set(0.75);
//			return false;
//		}
//	}

	/**
	 * Method that will drive the robot past the first line during autonomous,
	 * awarding 5 points. NOTE: This should not be called by AutoMiddle.cpp
	 * since the robot is starting in the center and runs the risk
	 * of crashing into the Switch if this method is executed.
	 */
	bool crossLine() {
		return driveStraight(Constants::D_LINE + Constants::CLENGTH, 0.5);
	}

	void stop() {
		drive->TankDrive(0.0, 0.0);
		drive->setLeftRight(0.0, 0.0);
	}

	virtual void init() = 0;

	virtual void run() = 0;

// These methods are common operations that will be used in autonomous
// run, and should be overridden for each Autonomous class
	virtual void sideSame() = 0;
	virtual void oppositeSide() = 0;
	/**
	 * left side switch
	 * right side scale
	 */
	virtual void leftRight() = 0;
	/**
	 * right side switch
	 * left side scale
	 */
	virtual void rightLeft() = 0;

	std::string start_position;
	CompChassis *drive;
	int autoState = 0;
protected:
// ^ The game specific String containing switch and scale positions

	void middleToRightSwitch() {

		drive->setGrippers(0.55);

		switch (autoState) {
		case 0:
			if (driveStraight(24, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(30, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(58, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(-12, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 3 finished at yaw = "
						<< drive->gyro->GetYaw() << std::endl;
				autoState++;
			}
			break;
		case 4:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(10, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 6:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 7:
			if (driveStraight(-12, 0.5)) {
				autoState++;
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		case 8:
			if (drive->liftFloor()) {
				autoState++;
			}
		}
	}
	void middleToLeftSwitch() {

		drive->setGrippers(0.55);

		switch (autoState) {
		case 0:
			if (driveStraight(24, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(-50, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(75, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(52, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 3 finished at yaw = "
						<< drive->gyro->GetYaw() << std::endl;
				autoState++;
			}
			break;
		case 4:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(42, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 6:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 7:
			if (driveStraight(-32, 0.5)) {
				autoState++;
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		case 8:
			if (drive->liftFloor()) {
				autoState++;
			}
		}
	}

	void leftToLeftSwitch() {

		switch (autoState) {
		case 0:
			if (driveStraight(142, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 2:
			if (turn(70, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 3:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 4:
			if (driveStraight(-24, 0.5)) {
				autoState++;
				std::cout << "State 5 complete" << std::endl;
			}
		}

	}
// goes around back
	void leftToRightSwitch() {

		drive->setGrippers(0.60);

		switch (autoState) {
		case 0:
			if (driveStraight(220, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(90, 0.45) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(144, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(60, 0.45) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 3 finished at yaw = "
						<< drive->gyro->GetYaw() << std::endl;
				autoState++;
			}
			break;
		case 4:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(6, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 6:
//			if (drive->shootCube() == true) {
//				autoState++;
//				std::cout << "State 6 complete" << std::endl;
//			}
			break;
		case 7:
			if (driveStraight(-5, 0.5)) {
				autoState++;
			}
		}
	}
	void leftToLeftScale() {

		switch (autoState) {
		case 0:
			if (driveStraight(238, 0.5) == true) { //252
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (drive->liftScale() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 2:
//			if (turn(35, 0.6) == true) {
//				drive->gyro->ZeroYaw();
//				drive->resetEncoders();
//				std::cout << "State 1 finished" << std::endl;
//			}
			autoState++;
			break;
		case 3:
//			if (driveStraight(12, 0.5)) {
//				drive->gyro->ZeroYaw();
//				drive->resetEncoders();
//				std::cout << "State 1 finished" << std::endl;
//			}
			autoState++;
			break;
		case 4:
//			if (drive->shootCube() == true) {
//				std::cout << "State 6 complete" << std::endl;
//			}
			autoState++;
			break;
		case 5:
//			if (driveStraight(-24, 0.5)) {
//				std::cout << "State 6 complete" << std::endl;
//			}
			autoState++;
		}
	}
	void leftToRightScale() {

		drive->setGrippers(0.55);

		switch (autoState) {
		case 0:
			if (driveStraight(200, 0.90) == true) {
				drive->gyro->ZeroYaw();
				std::cout << "State 0 finished" << std::endl;
				autoState++;
			}
			break;
		case 1:
			if (turn(77, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(190, 0.85) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
				autoState++;
			}
			break;
		case 3:
			if (turn(-60, 0.7) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 3 finished at yaw = "
						<< drive->gyro->GetYaw() << std::endl;
				autoState++;
			}
			break;
		case 4:
			if (drive->liftScale() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(30, 0.56) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 6:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 7:
			if (driveStraight(-18)) {
				autoState++;
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		}
	}

	void rightToLeftSwitch() {
		crossLine();
	}
// goes around back
	void rightToRightSwitch() {

		drive->setGrippers(0.50);

		switch (autoState) {
		case 0:
			if (driveStraight(122, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 2:
			if (turn(-70, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 3:
			if (driveStraight(8, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 5 complete" << std::endl;
			}
			break;
		case 4:
			if (drive->shootCube() == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(-24, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 5 complete" << std::endl;
			}
			break;
		case 6:
			if (turn(80, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
			}
		}
	}
	void rightToLeftScale() {
	}
	void rightToRightScale() {

		switch (autoState) {
		case 0:
			if (driveStraight(124, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (drive->liftScale() == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 2:
			if (turn(-45, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
			}
			break;
		case 3:
			if (drive->shootCube() == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 6 complete" << std::endl;
				autoState++;
			}
			break;
		case 4:
			if (driveStraight(-32, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 6 complete" << std::endl;
				autoState++;
			}
			break;
//		case 6:
//			if (drive->liftFloor()) {
//				autoState++;
//				std::cout << "State 6 complete" << std::endl;
//			}
//			break;
		}
	}
};

#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
