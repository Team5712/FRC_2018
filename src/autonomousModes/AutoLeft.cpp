/*
 * AutoLeft.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoLeft.h>
//#include "ctre/Phoenix.h"

AutoLeft::AutoLeft(BaseDrive *srcDrives) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoLeft::~AutoLeft() {

}

void AutoLeft::init() {



	char msg[80];
	sprintf(msg, "[AutoLeft Mode]: Game specific message: \"%s\"", start_position.c_str());
	DriverStation::ReportError(msg);

}

void AutoLeft::run() {
	double l_value = drive->getEncoderValues()[0];
	double r_value = drive->getEncoderValues()[1];


	// opposite sides, so don't attempt to place cubes
	if(start_position.at(0) == 'R' && start_position.at(1) == 'R') {
		crossLine(l_value, r_value);

	} // scale
	 else if(start_position.at(1) == 'L') {

		 driveForward(l_value, r_value, Constants::D_START_TO_SCALE - 30);
		 turn(40, 0.5);
		 // shoot out cube

		// switch
	} else if(start_position.at(0) == 'L') {

		driveForward(l_value, r_value, Constants::D_START_TO_SWITCH + ((Constants::D_START_TO_SWITCH_END - Constants::D_START_TO_SWITCH) / 2));
		turn(90, 0.5);
		// shoot out cube
	}


}


void AutoLeft::sideSame() {

}

void AutoLeft::oppositeSide() {

}
/**
 * left side switch
 * right side scale
 */
void AutoLeft::leftRight() {

}
/**
 * right side switch
 * left side scale
 */
void AutoLeft::rightLeft() {

}

