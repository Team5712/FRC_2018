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


	drive->shifter->Set(false);

//	if(start_position.at(1) == 'L') {
//		leftToLeftScale();
//	} else if(start_position.at(1) == 'R') {
//		leftToRightScale();
//	} else if(start_position.at(0) == 'L'){
//		leftToLeftSwitch();
//	} else {
//		crossLine();
//	}

	if(start_position.at(1) == 'L') {
		leftToLeftScale();
	} else if(start_position.at(1) == 'R') {
		leftToRightScale();
	} else {
		crossLine();
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

