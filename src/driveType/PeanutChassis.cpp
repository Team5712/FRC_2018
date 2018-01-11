#include <driveType/PeanutChassis.h>



PeanutChassis::PeanutChassis()
{
	
	LEFT_MOTOR_PORTS = new int[1];
	RIGHT_MOTOR_PORTS = new int[1];

	// TODO: Make sure sizeof returns the correct value
	//leftTalons
	for(int l = 0; l < sizeof(LEFT_MOTOR_PORTS); l++)
	{

	}

}

PeanutChassis::~PeanutChassis()
{

}

