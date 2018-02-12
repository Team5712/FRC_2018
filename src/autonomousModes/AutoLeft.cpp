/*
 * AutoLeft.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoLeft.h>
//#include "ctre/Phoenix.h"

AutoLeft::AutoLeft(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoLeft::~AutoLeft() {

}

void AutoLeft::init() {

	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	char msg[80];
	sprintf(msg, "[AutoLeft Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);

}

void AutoLeft::run() {


	// opposite sides, so don't attempt to place cubes
	if(positions.at(0) == 'R' && positions.at(1) == 'R') {
		crossLine();

		// scale
	} else if(positions.at(1) == 'L') {

		// switch
	} else if(positions.at(0) == 'L') {

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

