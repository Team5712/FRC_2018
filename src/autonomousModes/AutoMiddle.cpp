/*
 * AutoMiddle.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoMiddle.h>

AutoMiddle::AutoMiddle(BaseDrive *srcDrive, std::string pos) {
	
	start_position = pos;
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoMiddle::~AutoMiddle() {

}

void AutoMiddle::init() {


	char msg[80];
	sprintf(msg, "[AutoMiddle Mode]: Game specific message: \"%s\"", positions.c_str());
	DriverStation::ReportError(msg);
}


void AutoMiddle::run()
{

	if(start_position.empty()) {
		crossLine();
	}

	// Store the positions of the switch and scale
	positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	// switch right
	if(positions.at(0) == 'R') {

		driveForward(10, 0.5, 0.5);
		turn(45, 0.5);
		driveForward(40, 0.5, 0.5);
		turn(-45, 0.5);
		driveForward(20, 0.5, 0.5);
	}
	// switch left
	if(positions.at(0) == 'L') {

		driveForward(10, 0.5, 0.5);
		turn(-45, 0.5);
		driveForward(40, 0.5, 0.5);
		turn(45, 0.5);
		driveForward(20, 0.5, 0.5);
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
