#ifndef SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_
#define SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_

#include "autonomousModes/AutoBase.hpp"
#include "../driveType/BaseDrive.hpp"

class AutoMiddle : public AutoBase {


public:
	AutoMiddle(CompChassis *drive, std::string);
	virtual ~AutoMiddle();

	int ratioCounter = 0;
	// ---- AUTONOMOUS FUNCTIONS ----
	void run() override;
	void init() override;
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


	double lift_potScale = 20;
	double lift_potSwitch = 240;
	double lift_potFloor = 400;
};

#endif /* SRC_AUTONOMOUSMODES_AUTOMIDDLE_H_ */
