/*
 * SDInterface.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#include "SDInterface.h"

SDInterface::SDInterface() {
	
}

SDInterface::~SDInterface() {

}

StartingPosition SDInterface::getStartingPosition() {
	chooser.AddDefault(auto_name_left, auto_name_left);
	chooser.AddObject(auto_name_left, auto_name_left);

	chooser.AddDefault(auto_name_middle, auto_name_middle);
	chooser.AddObject(auto_name_middle, auto_name_middle);

	chooser.AddDefault(auto_name_right, auto_name_right);
	chooser.AddObject(auto_name_right, auto_name_right);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	if (chooser.GetSelected() == "Left") {
		return StartingPosition::LEFT;
	}
	if (chooser.GetSelected() == "Middle") {
		return StartingPosition::MIDDLE;
	}
	if (chooser.GetSelected() == "Right") {
		return StartingPosition::RIGHT;
	}

}

