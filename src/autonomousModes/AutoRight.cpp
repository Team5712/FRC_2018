/*
 * AutoRight.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoRight.h>
//#include "ctre/Phoenix.h"

AutoRight::AutoRight(BaseDrive *srcDrive, std::string pos) {
	
	start_position = pos;
	// Drive from AutoBase.hpp
	drive = srcDrive;
}

AutoRight::~AutoRight() {

}


void AutoRight::init() {



	char msg[80];
	sprintf(msg, "[AutoRight Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);

}


void AutoRight::run()
{
	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	// opposite sides
	if(positions.at(0) == 'L' && positions.at(1) == 'L') {
		crossLine();
		// scale
	} else if(positions.at(1) == 'R') {

		 driveForward(Constants::D_START_TO_SCALE - 30);
		 turn(-40, 0.5);

		// switch
	} else if(positions.at(0) == 'R') {

		driveForward(Constants::D_START_TO_SWITCH + ((Constants::D_START_TO_SWITCH_END - Constants::D_START_TO_SWITCH) / 2));
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
