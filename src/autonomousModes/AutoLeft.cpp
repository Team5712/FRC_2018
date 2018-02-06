/*
 * AutoLeft.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoLeft.h>

AutoLeft::AutoLeft(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoLeft::~AutoLeft() {

}

void AutoLeft::init() {
	char str2[80];
	sprintf(str2, "value of leftTraveled:");
	DriverStation::ReportError(str2);

	std::string positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();


//	// opposite sides
//	if(positions.at(0) == "R" && positions.at(1) == "R") {
//		crossLine();
//
//		// scale
//	} else if(positions.at(1) == "L") {
//
//		// switch
//	} else if(positions.at(0) == "L") {
//
//	}


}
void AutoLeft::run() {

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

