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
	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	char msg[80];
	sprintf(msg, "[AutoMiddle Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);
}


void AutoMiddle::run()
{
	// switch right
	if(positions.at(0) == 'R') {

	}
	// switch left
	if(positions.at(0) == 'L') {

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
