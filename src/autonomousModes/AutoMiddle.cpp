/*
 * AutoMiddle.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include <autonomousModes/AutoMiddle.h>

AutoMiddle::AutoMiddle(BaseDrive *srcDrive) {
	
	// Drive from AutoBase.hpp
	drive = srcDrive;

}

AutoMiddle::~AutoMiddle() {

}

void AutoMiddle::init() {
	char str2[80];
	sprintf(str2, "value of leftTraveled:");
	DriverStation::ReportError(str2);
}


void AutoMiddle::run()
{

}

void AutoMiddle::sideSame()
{

}
void AutoMiddle::oppositeSide()
{

}
/**
 * left side switch
 * right side scale
 */
void AutoMiddle::leftRight()
{

}
/**
 * right side switch
 * left side scale
 */
void AutoMiddle::rightLeft()
{

}
