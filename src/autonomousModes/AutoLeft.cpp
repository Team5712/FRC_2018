/*
 * AutoLeft.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoLeft.h>
//#include "ctre/Phoenix.h"

AutoLeft::AutoLeft(CompChassis *srcDrive, std::string g) {

	start_position = g;
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoLeft::~AutoLeft() {

}

void AutoLeft::init() {

	drive->shifter->Set(true);

	char msg[80];
	sprintf(msg, "[AutoLeft Mode]: Game specific message: \"%s\"",
			start_position.c_str());
	DriverStation::ReportError(msg);
	drive->resetEncoders();
	drive->gyro->ZeroYaw();

}

void AutoLeft::run() {

	// test these then we can use um
//	if (start_position.at(1) == 'L') { //L
//		leftToLeftScale();
//	}
//	if (start_position.at(1) == 'R') {
//		leftToRightScale();
//	}
	//switch right
//	if (start_position.at(0) == 'L') {
//		leftToRightSwitch();
//
//	} else if (start_position.at(0) == 'R') { //switch left
//		leftToLeftSwitch();
//	}
	//drive->current_led = 0.31;

	drive->shifter->Set(true);

	if(start_position.at(0) == 'L') {
		leftToLeftSwitch();

		// if the switch is not on the left just cross line
	} else if(driveStraight(100)) {
		std::cout << "done";
		stop();
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

