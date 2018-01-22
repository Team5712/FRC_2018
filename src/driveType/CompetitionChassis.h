/*
 * CompetitionChassis.h
 *
 *  Created on: Jan 22, 2018
 *      Author: robotics
 */

#ifndef SRC_DRIVETYPE_COMPETITIONCHASSIS_H_
#define SRC_DRIVETYPE_COMPETITIONCHASSIS_H_

#include "BaseDrive.h"

class CompetitionChassis : public BaseDrive {
public:
	CompetitionChassis();
	void teleopPeriodic();
	virtual ~CompetitionChassis();
};

#endif /* SRC_DRIVETYPE_COMPETITIONCHASSIS_H_ */
