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
	drive->resetEncoders();
	drive->gyro->ZeroYaw();

	std::cout << "staring auto" << std::endl;
}

void AutoLeft::run() {


	std::cout << "print" << std::endl;

	if(start_position.at(0) == 'L') {
		leftToLeftSwitch();

		// if the switch is not on the left just cross line
	} else if(start_position.at(1) == 'L') {
		leftToLeftScale();

	} else {
		leftToLeftScale();
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

