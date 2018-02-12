/*
 * AutoLeft.h
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */



#ifndef SRC_AUTONOMOUSMODES_AUTOLEFT_H_
#define SRC_AUTONOMOUSMODES_AUTOLEFT_H_

#include <autonomousModes/AutoBase.hpp>
#include "../driveType/BaseDrive.hpp"

class AutoLeft : public AutoBase {
public:
	AutoLeft(BaseDrive *drive);
	virtual ~AutoLeft();

	// ---- AUTONOMOUS FUNCTIONS ----
	void init() override;
	void run() override;
	// ------------------------------

	void sideSame() override;
	void oppositeSide() override;
	/**
	 * left side switch
	 * right side scale
	 */
	void leftRight() override;
	/**
	 * right side switch
	 * left side scale
	 */
	void rightLeft() override;

};

#endif /* SRC_AUTONOMOUSMODES_AUTOLEFT_H_ */
