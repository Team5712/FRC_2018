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

void AutoLeft::init()
{
	char str2[80];
	sprintf(str2, "value of leftTraveled:");
	DriverStation::ReportError(str2);
}
void AutoLeft::run()
{

}

void AutoLeft::sideSame()
{

}
void AutoLeft::oppositeSide()
{

}
/**
 * left side switch
 * right side scale
 */
void AutoLeft::leftRight()
{

}
/**
 * right side switch
 * left side scale
 */
void AutoLeft::rightLeft()
{

}

