/*
 * AutoLeft.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoLeft.h>
//#include "ctre/Phoenix.h"

AutoLeft::AutoLeft(BaseDrive *srcDrive, std::string pos) {
	
	start_position = pos;
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoLeft::~AutoLeft() {

}

void AutoLeft::init() {



	char msg[80];
	sprintf(msg, "[AutoLeft Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);

}

void AutoLeft::run() {
	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	// opposite sides, so don't attempt to place cubes
	if(positions.at(0) == 'R' && positions.at(1) == 'R') {
		crossLine();

	} // scale
	 else if(positions.at(1) == 'L') {

		 driveForward(Constants::D_START_TO_SCALE - 30);
		 turn(40, 0.5);
		 // shoot out cube

		// switch
	} else if(positions.at(0) == 'L') {

		driveForward(Constants::D_START_TO_SWITCH + ((Constants::D_START_TO_SWITCH_END - Constants::D_START_TO_SWITCH) / 2));
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

