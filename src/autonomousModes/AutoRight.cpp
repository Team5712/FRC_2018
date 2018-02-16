/*
 * AutoRight.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoRight.h>
//#include "ctre/Phoenix.h"

AutoRight::AutoRight(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;
}

AutoRight::~AutoRight() {

}


void AutoRight::init() {



	char msg[80];
	sprintf(msg, "[AutoRight Mode]: Game specific message: \"%s\"", start_position.c_str());
	DriverStation::ReportError(msg);

}


void AutoRight::run()
{
	double l_value = drive->getEncoderValues()[0];
	double r_value = drive->getEncoderValues()[1];

	// opposite sides
	if(start_position.at(0) == 'L' && start_position.at(1) == 'L') {
		crossLine(l_value, r_value);
		// scale
	} else if(start_position.at(1) == 'R') {

		 driveForward(l_value, r_value, Constants::D_START_TO_SCALE - 30);
		 turn(-40, 0.5);

		// switch
	} else if(start_position.at(0) == 'R') {

		driveForward(l_value, r_value, Constants::D_START_TO_SWITCH + ((Constants::D_START_TO_SWITCH_END - Constants::D_START_TO_SWITCH) / 2));
		turn(-90, 0.5);
	}

}

void AutoRight::sideSame()
{

}
void AutoRight::oppositeSide()
{

}
/**
 * left side switch
 * right side scale
 */
void AutoRight::leftRight()
{

}
/**
 * right side switch
 * left side scale
 */
void AutoRight::rightLeft()
{

}
