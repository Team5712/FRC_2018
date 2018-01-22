#include <driveType/PeanutChassis.h>



PeanutChassis::PeanutChassis()
{
	
//	LEFT_MOTOR_PORTS = 1;
//	RIGHT_MOTOR_PORTS = 1;
//
//	// TODO: Make sure sizeof returns the correct value
//	//leftTalons
//	for(int l = 0; l < LEFT_MOTOR_PORTS; l++)
//	{
//
//	}


	talonLeft = new WPI_TalonSRX(2);
	talonRight = new WPI_TalonSRX(1);

	drive = new DifferentialDrive(*talonLeft, *talonRight);

	joystick = new Joystick(0);

}

void PeanutChassis::teleopPeriodic()
{
	char str[80];
	sprintf(str, "Message from peanut");
	DriverStation::ReportError(str);
}

PeanutChassis::~PeanutChassis()
{

}

