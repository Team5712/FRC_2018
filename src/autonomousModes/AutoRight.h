/*
 * AutoRight.h
 *
 *  Created on: Jan 29, 2018
 *      Author: robotics
 */

#ifndef SRC_AUTONOMOUSMODES_AUTORIGHT_H_
#define SRC_AUTONOMOUSMODES_AUTORIGHT_H_

#include "AutoBase.hpp"
#include "../driveType/BaseDrive.hpp"

class AutoRight : public AutoBase {

public:
	AutoRight(CompChassis *srcDrive, std::string);
	virtual ~AutoRight();

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

#endif /* SRC_AUTONOMOUSMODES_AUTORIGHT_H_ */
