/*
 * AutoMiddle.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoMiddle.h>

AutoMiddle::AutoMiddle(CompChassis *srcDrive, std::string start) {

	// Drive from AutoBase.hpp
	start_position = start;
	drive = srcDrive;

}

AutoMiddle::~AutoMiddle() {

}

void AutoMiddle::init() {

	drive->resetEncoders();
	drive->gyro->ZeroYaw();

	char msg[80];
	sprintf(msg, "[AutoMiddle Mode]: Game specific message: \"%s\"",
			start_position.c_str());
	DriverStation::ReportError(msg);
}

void AutoMiddle::run() {

	drive->shifter->Set(true);
	drive->current_led = 0.43;

	if (start_position.at(0) == 'L') {
		middleToLeftSwitch();

	} else if (start_position.at(0) == 'R') {
		middleToRightSwitch();
	}
}

void AutoMiddle::sideSame() {

}
void AutoMiddle::oppositeSide() {

}
/**
 * left side switch
 * right side scale
 */
void AutoMiddle::leftRight() {

}
/**
 * right side switch
 * left side scale
 */
void AutoMiddle::rightLeft() {

}
