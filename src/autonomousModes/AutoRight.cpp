/*
 * AutoRight.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoRight.h>

AutoRight::AutoRight(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;
}

AutoRight::~AutoRight() {
	// TODO Auto-generated destructor stub
}


void AutoRight::init() {
	char str2[80];
	sprintf(str2, "value of leftTraveled:");
	DriverStation::ReportError(str2);

	std::string positions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

//	// opposite sides
//	if(positions.at(0) == "L" && positions.at(1) == "L") {
//		crossLine();
//
//		// scale
//	} else if(positions.at(1) == "R") {
//
//		// switch
//	} else if(positions.at(0) == "R") {
//
//	}


}


void AutoRight::run()
{

}

void AutoRight::sideSame()
{

}
void AutoRight::oppositeSide()
{

}
/**
 * left side switch
 * right side scale
 */
void AutoRight::leftRight()
{

}
/**
 * right side switch
 * left side scale
 */
void AutoRight::rightLeft()
{

}
