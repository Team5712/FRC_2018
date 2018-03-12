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

Priority SDInterface::getPriority() {

	priority_chooser.AddDefault(priority_scale, priority_scale);
	priority_chooser.AddObject(priority_scale, priority_scale);

	priority_chooser.AddDefault(priority_switch, priority_switch);
	priority_chooser.AddObject(priority_switch, priority_switch);
	frc::SmartDashboard::PutData("Auto Modes", &priority_chooser);

	if(priority_chooser.GetSelected() == "Scale") {
		return Priority::SCALE;
	} else {
		return Priority::SWITCH;
	}

	return Priority::SCALE;
}

StartingPosition SDInterface::getStartingPosition() {
	position_chooser.AddDefault(auto_name_left, auto_name_left);
	position_chooser.AddObject(auto_name_left, auto_name_left);

	position_chooser.AddDefault(auto_name_middle, auto_name_middle);
	position_chooser.AddObject(auto_name_middle, auto_name_middle);

	position_chooser.AddDefault(auto_name_right, auto_name_right);
	position_chooser.AddObject(auto_name_right, auto_name_right);
	frc::SmartDashboard::PutData("Auto Modes", &position_chooser);

	if (position_chooser.GetSelected() == "Left") {
		return StartingPosition::LEFT;
	}
	if (position_chooser.GetSelected() == "Middle") {
		return StartingPosition::MIDDLE;
	}
	if (position_chooser.GetSelected() == "Right") {
		return StartingPosition::RIGHT;
	}

	return StartingPosition::DEFAULT;
}

