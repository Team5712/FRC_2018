/*
 * Constants.h
 *
 *  Created on: Jan 31, 2018
 *      Author: robotics
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include<cmath>

namespace Constants {

	unsigned const int PEANUT_TICKS = 515;
	unsigned const int PEANUT_WHEEL_RADIUS = 3;
	const float PRATIO = (PEANUT_TICKS / (2 * M_PI * PEANUT_WHEEL_RADIUS));

	// Constants
	unsigned const int CTICKS = 515;
	unsigned const int CWHEEL_RADIUS = 2.1875;
	const float CRATIO = (CTICKS / (2 * M_PI * CWHEEL_RADIUS));

	// if there is an differance in each side's driving capability, it will always be a factor of the other one, thus the ratio.
	double bias_ratio = 0.0;
	double bias_offset = 0.0;
}



#endif /* SRC_CONSTANTS_H_ */
