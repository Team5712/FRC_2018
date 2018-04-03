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

	drive->resetEncoders();
	drive->gyro->Reset();
}

AutoRight::~AutoRight() {

}

void AutoRight::run() {


	drive->led->Set(-0.99);

	if(driveStraight(100, 0.75)) {
		stop();
	}
//	rightToRightScale();
//	if(start_position.at(1) == 'R') {
//		rightToRightScale();
//	} else if(start_position.at(0) == 'R') {
//		rightToRightSwitch();
//	} else {
//		rightToRightScale();
//	}

}

void AutoRight::init() {
	drive->resetEncoders();
	drive->gyro->ZeroYaw();

	std::cout << "staring auto" << std::endl;
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
