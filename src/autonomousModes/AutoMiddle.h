#ifndef SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_
#define SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_

#include "autonomousModes/AutoBase.hpp"
#include "../driveType/BaseDrive.hpp"
#include "AHRS.h"

class AutoMiddle : public AutoBase {


public:
	std::string start_position;
	AutoMiddle(BaseDrive *drive, std::string position);
	virtual ~AutoMiddle();

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

#endif /* SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_ */
