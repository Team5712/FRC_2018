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

		timer->Reset();
		timer->Start();

		while((abs(drive->getEncoderValues()[0])) < e_left && abs((drive->getEncoderValues()[1]) < e_right) && timer->Get() < time)
		{
			drive->ArcadeDrive(left, right);
		}

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

		Timer *timer = new Timer();
		int currentL, currentR = 0;

		// Take a "snapshot" of the encoder values instead of resetting them
		currentL = drive->getEncoderValues()[0];
		currentR = drive->getEncoderValues()[1];

		timer->Reset();
		timer->Start();

		while((drive->getEncoderValues()[0] - abs(currentL)) < e_left && (drive->getEncoderValues()[1] - currentR < abs(e_right)))
		{
			drive->ArcadeDrive(left * Constants::bias_ratio + Constants::bias_offset, right * Constants::bias_ratio + Constants::bias_offset);
		}

	}

	// stop the robot and rotate it at a given angle
	void turn(double degrees)
	{
		drive->TankDrive(0.0, 0.0, false);

		while(drive->getGyroYaw()) {
			drive->TankDrive(0.0, 0.0, false);
		}

		drive->TankDrive(0.0, 0.0, false);
	}

	virtual void init() = 0;
	virtual void run() = 0;

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

};



#endif /* SRC_AUTONOMOUSMODES_AUTOBASE_HPP_ */
