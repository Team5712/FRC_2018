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

	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	char msg[80];
	sprintf(msg, "[AutoRight Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);

}


void AutoRight::run()
{
	// opposite sides
	if(positions.at(0) == 'L' && positions.at(1) == 'L') {
		crossLine();

		// scale
	} else if(positions.at(1) == 'R') {

		// switch
	} else if(positions.at(0) == 'R') {

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
