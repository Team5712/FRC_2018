#ifndef SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_
#define SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_

#include "autonomousModes/AutoBase.hpp"
#include "../driveType/BaseDrive.hpp"

class AutoMiddle : public AutoBase {


public:
	AutoMiddle(BaseDrive *drive);
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
