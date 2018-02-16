/*
 * AutoBase.hpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#ifndef SRC_AUTONOMOUSMODES_AUTOBASE_HPP_
#define SRC_AUTONOMOUSMODES_AUTOBASE_HPP_

#include "../driveType/BaseDrive.hpp"
#include "../Constants.h"
#include <iostream>
#include "ctre/Phoenix.h"
#include "WPILib.h"

class AutoBase
{
public:
	virtual ~AutoBase()
	{

	};

	/**
	 * drive at the specified motor output for a set amount of time and/or encoder value
	 */
	void timedDrive(double time, double left, double right, int e_left, int e_right)
	{

		Timer *timer = new Timer();
		int currentL, currentR = 0;

		// Take a "snapshot" of the encoder values instead of resetting them
		currentL = drive->getEncoderValues()[0];
		currentR = drive->getEncoderValues()[1];

		timer->Reset();
		timer->Start();

		while((abs(drive->getEncoderValues()[0] - currentL) < e_left) || (abs(drive->getEncoderValues()[1] - currentR) < e_right) || (timer->Get() < time))
		{
			drive->TankDrive(left, right);
		}
		// Stop the motors since the target / time was reached

		stop();
	}

	void timedDrive(double time, double left, double right)
	{

		Timer *timer = new Timer();

		timer->Reset();
		timer->Start();

		while(timer->Get() < time)
		{
			drive->TankDrive(left, right);
		}
		// When the loop ends, stop the motors
		stop();

	}

	void timedDrive(double left, double right, int e_left, int e_right)
	{

		int currentL, currentR = 0;

		// Take a "snapshot" of the encoder values instead of resetting them
		currentL = drive->getEncoderValues()[0];
		currentR = drive->getEncoderValues()[1];

		while((drive->getEncoderValues()[0] - abs(currentL) < e_left) && (drive->getEncoderValues()[1] - abs(currentR) < e_right))
		{
			drive->TankDrive(left * Constants::bias_ratio + Constants::bias_offset, right * Constants::bias_ratio + Constants::bias_offset);
		}
		stop(); // Reached the target, so stop the motors

	}

	// stop the robot and rotate it at a given angle
	void turn(double degrees, double power)
	{
		std::cout << "periodic" << std::endl;

		stop();
		drive->gyro->ZeroYaw();

		// turning right
		if(degrees > 0) {
			// give left motor power
			while(drive->getGyroYaw() < degrees) {
				std::cout << "[AutoBase.hpp][turn()] Gyro Yaw:" << drive->getGyroYaw() << std::endl;
				drive->TankDrive(abs(power), 0.0);
			}

		} else  {

			while(drive->getGyroYaw() > degrees) {
				drive->TankDrive(0.0, abs(power));
			}
		}

		stop();
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
	int driveForward(double l_value, double r_value, double inches, double left = 0.5, double right = 0.5) {
		stop();
		l_value = drive->getEncoderValues()[0] - l_value;
		r_value = drive->getEncoderValues()[1] - r_value;

		if(l_value < inches * Constants::CRATIO && r_value < inches * Constants::CRATIO) {
			drive->TankDrive(left , right);
			std::cout << "drive " << l_value << " " << r_value << std::endl;
			return 0;
		} else {
			std::cout << "finished" << std::endl;
			return 1;
		}
		/*finished
finished
finished
drive 0 40000
finished
finished
finished
finished
finished
drive 0 1.04292e+06
drive 0 49280
drive 1.2307e+09 928
finished
finished
finished
drive 0 83664
drive 0 56704
finished
drive 0 1.01584e+06
finished
drive 0 43416
drive 0 59704
finished
finished
finished
finished
		 * */


		stop();
	}

	/**
	 * Method that will drive the robot past the first line during autonomous,
	 * awarding 5 points. NOTE: This should not be called by AutoMiddle.cpp
	 * since the robot is starting in the center and runs the risk
	 * of crashing into the Switch if this method is executed.
	 */
	int crossLine(double l_value, double r_value) {
		return this->driveForward(l_value, r_value, Constants::D_LINE + Constants::CLENGTH, 0.5, 0.5);
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


protected:
	BaseDrive *drive;
	std::string start_position;
	// ^ The game specific String containing switch and scale positions

};



#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
