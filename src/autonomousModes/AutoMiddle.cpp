/*
 * AutoMiddle.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoMiddle.h>

AutoMiddle::AutoMiddle(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoMiddle::~AutoMiddle() {

}

void AutoMiddle::init() {


	char msg[80];
	sprintf(msg, "[AutoMiddle Mode]: Game specific message: \"%s\"", start_position.c_str());
	DriverStation::ReportError(msg);
}


void AutoMiddle::run()
{
	double l_value = drive->getEncoderValues()[0];
	double r_value = drive->getEncoderValues()[1];

	// TODO: Why "or true"?
	// Unsafe for the middle position?
	if(start_position.empty() || true) {

		// 1 meaning we've actually crossed the line
		if(crossLine(l_value, r_value) == 1) {
			stop();
		}

	} else {

		// switch right
		if(start_position.at(0) == 'R') {

			driveForward(10, 0.5, 0.5);
			turn(45, 0.5);
			driveForward(40, 0.5, 0.5);
			turn(-45, 0.5);
			driveForward(20, 0.5, 0.5);
		}
		// switch left
		if(start_position.at(0) == 'L') {

			driveForward(10, 0.5, 0.5);
			turn(-45, 0.5);
			driveForward(40, 0.5, 0.5);
			turn(45, 0.5);
			driveForward(20, 0.5, 0.5);
		}
	}

}

void AutoMiddle::sideSame()
{

}
void AutoMiddle::oppositeSide()
{

}
/**
 * left side switch
 * right side scale
 */
void AutoMiddle::leftRight()
{

}
/**
 * right side switch
 * left side scale
 */
void AutoMiddle::rightLeft()
{

}
