/*
 * Constants.h
 *
 *  Created on: Jan 31, 2018
 *      Author: robotics
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

//#include <cmath>

class Constants {
public:

	// Peanut chassis constants
	static unsigned const int PEANUT_TICKS = 515;
	static unsigned const int PEANUT_WHEEL_RADIUS = 3;
	static const constexpr float PRATIO = (Constants::PEANUT_TICKS
			/ (2 * M_PI * Constants::PEANUT_WHEEL_RADIUS));

	// Competition chassis constants
	static unsigned const int TICKSPERREVOLUTION = 125;
	static unsigned const int CWHEEL_RADIUS = 2.1075;
	// To determine ticks to inch, use this link:
	// http://www.robotc.net/wikiarchive/Tutorials/Arduino_Projects/Mobile_Robotics/VEX/Using_encoders_to_drive_some_distance
//	static const constexpr float RATIO = 360 / (Constants::CWHEEL_RADIUS * 2 * M_PI);
	static const constexpr float RATIO = 168.04;
	//static const constexpr float CRATIO = (Constants::CTICKS / (2 * M_PI * Constants::CWHEEL_RADIUS));
	static const constexpr double CLENGTH = 20.0; // Length of the robot from front to back TODO: MEASURE THIS


	// if there is an differance in each side's driving capability, it will always be a factor of the other one, thus the ratio.
	static double const constexpr bias_ratio = 0.05;
	static double const constexpr bias_offset = 0.0;

	// distances given by FRC, we may not use these in code but they are nice to write down
	// https://firstfrc.blob.core.windows.net/frc2018/Drawings/LayoutandMarkingDiagram.pdf
	// of of these measurements are in inches

	// this is the distance from the left or right starting position to the middle of the platform area where you climb or park.
	static double const constexpr D_START_TO_MID_PLATFORM = 288.00;

	// this is the distance from the left or right starting position to the scale.
	static double const constexpr D_START_TO_SCALE = 299.65;

	// this is the distance from any starting position to the switch.
	static double const constexpr D_START_TO_SWITCH = 140.00;

	// this is the distance from any starting position to the end of the switch. this means the width of the switch is (end - start)
	static double const constexpr D_START_TO_SWITCH_END = 196.00;

	// this is the distance from any starting position to the line that you need to cross in order to get 5 points
	static double const constexpr D_LINE = 120.00;
};

#endif /* SRC_CONSTANTS_H_ */
