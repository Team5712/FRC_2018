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

//	if (start_position.at(0) == 'L') {
//		middleToLeftSwitch();
//
//	} else if (start_position.at(0) == 'R') {
//		middleToRightSwitch();
//	}

	if(driveStraight(120)) {
		stop();
		std::cout << "stop";
	}

//	std::cout << "left: " << drive->getLeftValue() << std::endl;
//	std::cout << "right: " << drive->getRightValue() << std::endl;
//
//	std::cout << "goal " << 5  * drive->mm.TICKS_PER_FOOT << std::endl;
//
//	drive->l_master->Set(ControlMode::MotionMagic, 5  * drive->mm.TICKS_PER_FOOT);
//	drive->r_master->Set(ControlMode::MotionMagic, 5 * drive->mm.TICKS_PER_FOOT);


//	drive->l_master->SetSelectedSensorPosition(5 * Constants::RATIO, 0, drive->mm.TIMEOUT);
//	drive->r_master->SetSelectedSensorPosition(5 * Constants::RATIO, 0, drive->mm.TIMEOUT);
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

