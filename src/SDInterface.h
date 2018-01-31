/*
 * SDInterface.h
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#ifndef SDINTERFACE_H_
#define SDINTERFACE_H_

#include<string>
#include <WPILib.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "ctre/phoenix/MotorControl/CAN/TalonSRX.h"
#include "StartingPosition.h"

class SDInterface {
public:
	SDInterface();


	frc::LiveWindow* lw = LiveWindow::GetInstance();
	frc::SendableChooser<std::string> chooser;
	const std::string auto_name_left = "Left";
	const std::string auto_name_middle = "Middle";
	const std::string auto_name_right = "Right";
	std::string autoSelected;


	StartingPosition getStartingPosition();


	virtual ~SDInterface();
};

#endif /* SDINTERFACE_H_ */
