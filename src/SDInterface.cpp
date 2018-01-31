/*
 * SDInterface.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include "SDInterface.h"

SDInterface::SDInterface() {
	// TODO Auto-generated constructor stub
	
}

SDInterface::~SDInterface() {
	// TODO Auto-generated destructor stub
}

StartingPosition SDInterface::getStartingPosition() {
	chooser.AddDefault(auto_name_left, auto_name_left);
	chooser.AddObject(auto_name_left, auto_name_left);

	chooser.AddDefault(auto_name_middle, auto_name_middle);
	chooser.AddObject(auto_name_middle, auto_name_middle);

	chooser.AddDefault(auto_name_right, auto_name_right);
	chooser.AddObject(auto_name_right, auto_name_right);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);


	return StartingPosition::MIDDLE;
}



