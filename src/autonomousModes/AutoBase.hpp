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
		drive->TankDrive(0.0, 0.0);

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
		drive->TankDrive(0.0, 0.0);

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
		drive->TankDrive(0.0, 0.0); // Reached the target, so stop the motors

	}

	// stop the robot and rotate it at a given angle
	void turn(double degrees, double power)
	{
		std::cout << "periodic" << std::endl;

		drive->TankDrive(0.0, 0.0);
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

		drive->TankDrive(0.0, 0.0);
	}

	/**
	 * This method will drive the robot forward the given number of inches.
	 * The distance is calculated based on the ratio defined in Constants.h.
	 * In addition, two optional pramaeters may be supplied to dictate
	 * how fast the robot drives forward. NOTE: This only works for
	 * forward driving at the moment.
	 *
	 * @param inches
	 * the distance in inches to travel forward
	 * @param left
	 * the power of the left motors on the robot
	 * @param right
	 * the power of the right motors on the robot
	 */
	void driveForward(double inches, double left = 0.5, double right = 0.5) {
		drive->TankDrive(0.0 , 0.0);

		while(drive->getEncoderValues()[0] < inches * Constants::CRATIO && drive->getEncoderValues()[1] < inches * Constants::CRATIO) {
			drive->TankDrive(left , right);
		}

		drive->TankDrive(0.0 , 0.0);
	}

	/**
	 * Method that will drive the robot past the first line during autonomous,
	 * awarding 5 points. NOTE: This should not be called by AutoMiddle.cpp
	 * since the robot is starting in the center and runs the risk
	 * of crashing into the Switch if this method is executed.
	 */
	void crossLine() {
		this->driveForward(Constants::D_LINE + Constants::CLENGTH);
	}

	virtual void init() = 0;
	virtual void run() = 0;

	// These methods are common operations that will be used in autonomous
	// run, and should be overriten for each Autonomous class
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
	std::string positions;
	// ^ The game specific String containing switch and scale positions

};



#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
