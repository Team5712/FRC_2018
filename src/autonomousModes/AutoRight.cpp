/*
 * AutoRight.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoRight.h>
//#include "ctre/Phoenix.h"

AutoRight::AutoRight(CompChassis *srcDrive, std::string g) {

	// Drive from AutoBase.hpp
	start_position = g;
	drive = srcDrive;
}

AutoRight::~AutoRight() {

}

void AutoRight::init() {

	char msg[80];
	sprintf(msg, "[AutoRight Mode]: Game specific message: \"%s\"",
			start_position.c_str());
	DriverStation::ReportError(msg);

}

void AutoRight::run() {

//	if(start_position.at(0) == 'L') {
//		rightToLeftSwitch();
//	}
//	if(start_position.at(0) == 'R') {
//		rightToRightSwitch();
//	}
//	if(start_position.at(1) == 'L') {
//		rightToLeftScale();
//	}
//	if(start_position.at(1) == 'R') {
//		rightToRightScale();
//	}

	drive->shifter->Set(true);

	if(start_position.at(0) == 'R') {
		rightToRightSwitch();

		// if the switch is not on the left just cross line
	} else if(driveStraight(100)) {
		std::cout << "done";
		stop();
	}

}

void AutoRight::sideSame() {

}
void AutoRight::oppositeSide() {

}
/**
 * left side switch
 * right side scale
 */
void AutoRight::leftRight() {

}
/**
 * right side switch
 * left side scale
 */
void AutoRight::rightLeft() {

}
