/*
 * CompetitionChassis.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: robotics
 */

#include <driveType/CompetitionChassis.h>

CompetitionChassis::CompetitionChassis() {
	// TODO Auto-generated constructor stub
	
}

void CompetitionChassis::teleopPeriodic() {
	char str[80];
	sprintf(str, "Message from competition");
	DriverStation::ReportError(str);
}

CompetitionChassis::~CompetitionChassis() {
	// TODO Auto-generated destructor stub
}

