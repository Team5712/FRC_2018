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

	double forward_power = 0.60;
	double turn_power = 0.5;

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
	bool turn(double degrees, double power, double range = 5) {
		//std::cout << "periodic" << std::endl;

		double yaw = drive->gyro->GetYaw();

		// turn left
		if (yaw > degrees + range) {
			drive->TankDrive(power, -power);
			return false;
		} else if (yaw < degrees - range) {
//			std::cout << "[AutoBase.hpp][turn()] 1st Gyro Yaw:" << yaw << std::endl;
			drive->TankDrive(-power, power);
			return false;
		} else {
			std::cout << "[AutoBase.hpp][turn()] Finished!" << std::endl;
			stop();
			return true;
		}
	}

	/**
	 * This method will drive the robot forward the given number of inches.
	 * The distance is calculated based on the ratio defined in Constants.h.
	 * In addition, two optional pramaeters may be supplied to dictate
	 * how fast the robot drives forward. NOTE: This only works for
	 * forward driving at the moment.
	 *
	 * @param l_value
	 * this is the value of the left encoder recorded before this function is called. The value of
	 * this variable will remain the same until this function is done being called within the loop
	 * the use case of this is denoted by
	 * l_value = leftEncoder[0] - value;
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
	bool driveStraight(double inches, double power = 0.50) {

//		std::cout << "drive " << drive->getLeftValue() << " and "
//				<< drive->getRightValue() << std::endl;

		if (inches > 0) {
			if (abs(drive->getLeftValue()) < Constants::RATIO * inches
					|| abs(drive->getRightValue())
							< Constants::RATIO * inches) {
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
			if (driveStraight(24) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(45, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(54) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(-30, 0.6) == true) {
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
			if (driveStraight(12) == true) {
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
			if (driveStraight(-12)) {
				autoState++;
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		}
	}
	void middleToLeftSwitch() {

		drive->setGrippers(0.55);

		switch (autoState) {
		case 0:
			if (driveStraight(24) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(-45, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(98) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
			}
			break;
		case 3:
			if (turn(30, 0.6) == true) {
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
			if (driveStraight(12) == true) {
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
			if (driveStraight(-12)) {
				autoState++;
				std::cout << "State 7 complete" << std::endl;
			}
			break;
		}
	}

	void leftToLeftSwitch() {

		switch (autoState) {
		case 0:
			if (driveStraight(168) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(90, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 3:
			if (driveStraight(24) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 4:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(-12)) {
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
			if (driveStraight(220) == true) {
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
			if (driveStraight(144) == true) {
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
			if (driveStraight(6) == true) {
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
			if (driveStraight(-5)) {
				autoState++;
			}
		}
	}
	void leftToLeftScale() {

		switch (autoState) {
		case 0:
			if (driveStraight(348) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(90, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (drive->liftScale() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 3:
			if (driveStraight(18) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 4:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		}
	}
	void leftToRightScale() {

		drive->setGrippers(0.55);

		switch (autoState) {
		case 0:
			if (driveStraight(230) == true) {
				drive->gyro->ZeroYaw();
				std::cout << "State 0 finished" << std::endl;
				autoState++;
			}
			break;
		case 1:
			if (turn(90, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (driveStraight(208) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 2 finished" << std::endl;
				autoState++;
			}
			break;
		case 3:
			if (turn(-90, 0.6) == true) {
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
			if (driveStraight(48) == true) {
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
			if (driveStraight(168) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				autoState++;
				std::cout << "State 0 finished" << std::endl;
			}
			break;
		case 1:
			if (turn(-90, 0.6) == true) {
				drive->gyro->ZeroYaw();
				drive->resetEncoders();
				std::cout << "State 1 finished" << std::endl;
				autoState++;
			}
			break;
		case 2:
			if (drive->liftSwitch() == true) {
				autoState++;
				std::cout << "State 4 finished" << std::endl;
			}
			break;
		case 3:
			if (driveStraight(24) == true) {
				drive->gyro->ZeroYaw();
				autoState++;
				std::cout << "State 5 finished" << std::endl;
			}
			break;
		case 4:
			if (drive->shootCube() == true) {
				autoState++;
				std::cout << "State 6 complete" << std::endl;
			}
			break;
		case 5:
			if (driveStraight(-12)) {
				autoState++;
				std::cout << "State 5 complete" << std::endl;
			}
		}
	}
	void rightToLeftScale() {
		crossLine();
	}
	void rightToRightScale() {
		crossLine();
	}

};

#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
