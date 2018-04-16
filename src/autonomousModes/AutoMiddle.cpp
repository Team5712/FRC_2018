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

void AutoMiddle::init() {
	drive->resetEncoders();
	drive->gyro->ZeroYaw();

	std::cout << "staring auto" << std::endl;
}

AutoMiddle::~AutoMiddle() {

}

void AutoMiddle::run() {


	drive->shifter->Set(false);

	// TODO drive forward more on first left turn, turn and turn less
	if (start_position.at(0) == 'L') {
		middleToLeftSwitch();

	} else if (start_position.at(0) == 'R') {
		middleToRightSwitch();
	}

//	std::cout << "left: " << drive->getLeftValue()  / Constants::RATIO << std::endl;
//	std::cout << "right: " << drive->getRightValue() / Constants::RATIO << std::endl;
//
//	std::cout << "goal " << 5  * drive->mm.TICKS_PER_FOOT << std::endl;
//
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

