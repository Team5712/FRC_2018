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

		std::cout << yaw << std::endl;

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

	struct Acceleration {

		void reset() {
			this->power = 0;
			this->time = 0;
		}

		// measured in 20 ms at a time
		double time = 0;
		// (ramp_time_goal / 20) * ramp_power will be how much you will ramp up
		double rate = 0.0075;
		double time_goal = 2000;
		// how much power we're giving the motors during ramp and deacceleration
		double power = 0;
		// minumum power that we add to pid_power
		double min_power = 0.05;
	} ramp;

	struct Deacceleration {
		// (ramp_time_goal / 20) * ramp_power will be how much you will ramp up
		double rate = 0.0075;
		// how much power we're subtracting from the motors during ramp and deacceleration
		double power = 0;
		// minumum power that we add to pid_power

		// we should be at the target by the time we get to our encoder target
		double target_power = 0.10;

		double l_previous = 0;
		double r_previous = 0;

		bool isSlowing = false;
	} slow;

	/*
	 *
	 * this will effectively "reset" the value of the encoder
	 *
	 *
	 * example slow down
	 *
	 * inches: 100
	 * power: 0.5
	 * percentage: 0.75
	 *
	 * then de-increment our power equal to the newpower every inch
	 * newpower = (power - target) / inch
	 *
	 * 0.5 power
	 * 0.10 target
	 * 25 inches left
	 * \\\\\\\\\\\\\\\\\\\\\\\\\
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
	bool driveStraightRamp(double inches, double power = 0.5, double ramp_time =
			0) {

		// to get distance to destination given percentage
		// distance = inches - (inches * percent)

		ramp.time += 20;

		// inches will now be in ticks so we can reference them with the encoders
		inches *= Constants::RATIO;
		double yaw = drive->gyro->GetYaw();
		double error = abs(yaw);
		double gyro_correction = 0.15;
		double gyro_range = 2;
		// the point where we want to start slowing down

//		std::cout << "left " << drive->getLeftValue() << std::endl;
//		std::cout << "right " << drive->getRightValue() << std::endl;
//		std::cout << "goal " << inches << std::endl;

		// if we haven't exeeded our time goal for ramping up
		if (ramp.time_goal != 0 && ramp.time < ramp.time_goal) {
			ramp.power += ramp.rate;
			std::cout << "ramping up - " << ramp.power << std::endl;
		} else {
			std::cout << "power " << power << std::endl;
			ramp.power = power;
		}

//		std::cout << "correction: " << gyro_correction << std::endl;

		// if were drifting right
		if (yaw > gyro_range) {

			// if we are told to go forward
			if (inches > 0) {

				// if our current ticks are less than how far we need to go
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					// if we are still ramping up to our goal
					if (ramp.time_goal != 0 && ramp.time < ramp.time_goal) {
						drive->setLeftRight(ramp.power + ramp.min_power,
								ramp.min_power + ramp.power + gyro_correction);
						// if we're not ramping up and were ready to slow downif
					} else {
						drive->setLeftRight(power, power + gyro_correction);
					}
					return false;
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() > inches
						|| drive->getRightValue() > inches) {
					drive->setLeftRight(-power - gyro_correction, -power);
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			// if we're drifting left
		} else if (yaw < -gyro_range) {

			if (inches > 0) {
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					if (ramp.time_goal != 0 && ramp.time < ramp.time_goal) {
						drive->setLeftRight(
								ramp.power + ramp.min_power + gyro_correction,
								ramp.min_power + ramp.power);
						// if we're not ramping up and were ready to slow down
					} else {
						drive->setLeftRight(power + gyro_correction, power);
					}
					return false;
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() > inches
						|| drive->getRightValue() > inches) {
					drive->setLeftRight(-power, -power - gyro_correction);
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			// if we're in the target gyro range
		} else {
			if (inches > 0) {
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					// if we are still ramping up to our goal
					if (ramp.time_goal != 0 && ramp.time < ramp.time_goal) {
						drive->setLeftRight(ramp.power + ramp.min_power,
								ramp.min_power + ramp.power);
					} else {
						drive->setLeftRight(power + gyro_correction, power);
					}

					drive->setLeftRight(power, power);
					return false;
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (drive->getLeftValue() > Constants::RATIO * inches
						|| drive->getRightValue() > Constants::RATIO * inches) {
					drive->setLeftRight(-power, -power);
				} else {
					ramp.reset();
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
		}

		return false;
	}

	bool driveStraight(double inches, double power = 0.5,
			double ramp_time = 0) {
		// to get distance to destination given percentage
		// distance = inches - (inches * percent)

		// inches will now be in ticks so we can reference them with the encoders
		inches *= Constants::RATIO;
		double yaw = drive->gyro->GetYaw();
		double error = abs(yaw);
		double gyro_correction = 0.15;
		double gyro_range = 2;
		// the point where we want to start slowing down

//		std::cout << "correction: " << gyro_correction << std::endl;

		// if were drifting right
		if (yaw > gyro_range) {

			// if we are told to go forward
			if (inches > 0) {

				// if our current ticks are less than how far we need to go
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					drive->setLeftRight(power, power + gyro_correction);
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (abs(drive->getLeftValue()) < abs(inches)
						|| abs(drive->getRightValue()) < abs(inches)) {
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
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					drive->setLeftRight(power + gyro_correction, power);
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (abs(drive->getLeftValue()) < abs(inches)
						|| abs(drive->getRightValue()) < abs(inches)) {
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
				if (abs(drive->getLeftValue()) < inches
						|| abs(drive->getRightValue()) < inches) {

					drive->setLeftRight(power + gyro_correction, power);

					drive->setLeftRight(power, power);
					return false;
				} else {
					std::cout << "stop" << std::endl;
					stop();
					return true;
				}
			}
			if (inches < 0) {
				if (abs(drive->getLeftValue()) < abs(inches)
						|| abs(drive->getRightValue()) < abs(inches)) {
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

		switch (autoState) {
		case 0:
			if (driveStraight(30, 0.5) == true) {
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
				drive->resetEncoders();
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
			if (driveStraight(-24, 0.5)) {
				autoState++;
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		case 8:
			if (turn(-25, 0.5)) {
				autoState++;
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
			}
			break;
		case 9:
			if (drive->liftFloor()) {
				autoState++;
				drive->isIntakingAuto = true;
			}
		}
	}
	void middleToLeftSwitch() {

		switch (autoState) {
		case 0:
			if (driveStraight(20, 0.5)) {
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
			if (driveStraight(90, 0.5) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(38, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
			}
			break;
		case 4:
			if (drive->liftSwitch() == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(24, 0.5) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
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
			if (driveStraight(-30, 0.5)) {
				autoState++;
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		case 8:
			if (turn(30, 0.5)) {
				autoState++;
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
			}
			break;
		case 9:
			if (drive->liftFloor()) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				drive->isIntakingAuto = true;
			}
			break;
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
			if (driveStraightRamp(284, 0.70) == true) { //252
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
			if (turn(80, 0.45) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 1 finished" << std::endl;
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
			if (turn(75, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 6 complete" << std::endl;
				autoState++;
			}
		}
	}
	void leftToRightScale() {

		switch (autoState) {
		case 0:
			if (driveStraightRamp(198, 0.75) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 0 finished" << std::endl;
				autoState++;
			}
			break;
		case 1:
			if (turn(81, 0.5) == true) {
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
			if (turn(-78, 0.5) == true) {
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
			if (driveStraight(25, 0.40) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
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

		switch (autoState) {
		case 0:
			if (driveStraightRamp(198, 0.75) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 0 finished" << std::endl;
				autoState++;
			}
			break;
		case 1:
			if (turn(-81, 0.5) == true) {
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
			if (turn(78, 0.5) == true) {
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
			if (driveStraight(25, 0.40) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
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
	void rightToRightScale() {

		switch (autoState) {
		case 0:
			if (driveStraightRamp(278, 0.70) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				stop();
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
			if (turn(-75, 0.45)) {
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
			if (turn(-75, 0.5)) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 6 complete" << std::endl;
				autoState++;
			}
			break;
		}
	}
};

#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
