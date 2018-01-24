#include "driveType/PeanutChassis.h"
#include "WPILib.h"

PeanutChassis::PeanutChassis()
{
	leftTalon = new WPI_TalonSRX(2);
	rightTalon = new WPI_TalonSRX(1);

	drive = new DifferentialDrive(*leftTalon, *rightTalon);

	joystick = new Joystick(0);

	timer = new Timer();

	pot = new AnalogPotentiometer(0, 1, 0);

	// Define the settings for the encoders
	leftTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	rightTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

}

PeanutChassis::~PeanutChassis()
{
	// De-reference pointers
	delete leftTalon;
	delete rightTalon;

	delete timer;
}

// ---- ROBOT.CPP METHODS ----

void PeanutChassis::autonomousInit()
{

	leftTalon->SetSelectedSensorPosition(0, 0, 10);
	rightTalon->SetSelectedSensorPosition(0, 0, 10);

	timer->Reset();
	timer->Start();

}

void PeanutChassis::autonomousPeriodic()
{

	if(timer->Get() < 1.0)
	{
		TankDrive(0.3, -0.3);
	} else
	{
		TankDrive(0.0, 0.0);
	}

	// NOTE: The encoder values return as an INTEGER, not a double
	char str[80];
	sprintf(str, "L = %d, R = %d", leftTalon->GetSelectedSensorPosition(0), rightTalon->GetSelectedSensorPosition(0));
	DriverStation::ReportError(str);

}

void PeanutChassis::teleopInit()
{

}

void PeanutChassis::teleopPeriodic()
{
//	char str[80];
//	sprintf(str, "Message from peanut");
//	DriverStation::ReportError("Message from Peanut chassis");

	ArcadeDrive(getJoystickValue(1), getJoystickValue(0));

	double val = pot->Get();

	char str[128];
	sprintf(str,"Snoopy is smokin pot at: %f", val);
	DriverStation::ReportError(str);

}

// ---- END ROBOT.CPP METHODS ----

// ---- DIFFERENTIAL_DRIVE METHODS ----


/**
 * Arcade drive method for differential drive platform. The calculated values
 * will be squared to decrease sensitivity at low speeds. This method
 * will use the motors and configurations required by the Peanut Chassis.
 *
 * @param xSpeed
 * the robot's speed along the X axis [-1.0..1.0]. Forward is positive.
 * @param zRotation
 * the robot's rotation rate around the Z axis [-1.0..1.0]. Clockwise is positive.
 * @param squaredInputs
 * default value is true, which will help limit sensitivity at slow speeds.
 */
void PeanutChassis::ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs)
{
	drive->ArcadeDrive(xSpeed, zRotation, squaredInputs);
}

/**
 * The rotation argument controls the curvature of the robot's path rather
 * than its rate of heading change. This makes the robot more controllable
 * at high speeds. Also handles the robot's quick turn functionality
 * - "quick turn" overrides constant-curvature turning for turn-in-place
 * maneuvers.
 *
 * The main difference between this method and ArcadeDrive is that the
 * zRotation equals the heading of the robot in this method; in ArcadeDrive,
 * zRotation specifies how much the robot will turn.
 *
 * @param xSpeed
 * the robot's speed along the X axis [-1.0..1.0]. Forward is positive.
 * @param zRotation
 * the robot's heading around the Z axis [-1.0..1.0]. Clockwise is positive.
 * @param isQuickTurn
 * if set, overrides constant-curvature turning for turn-in-place maneuvers.
 *
 */
void PeanutChassis::CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn)
{
	drive->CurvatureDrive(xSpeed, zRotation, isQuickTurn);
}

/**
 * Drive that explicitly takes the power of the left and right sides of the
 * robot. squaredInputs will help reduce sensitivity if set to true.
 *
 * @param leftSpeed
 * the speed of the left side of the robot
 * @param rightSpeed
 * the speed of the right side of the robot
 * @param squredInputs
 * defalut value is true. This will control how sensitive the robot is
 */
void PeanutChassis::TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs)
{
	drive->TankDrive(leftSpeed, rightSpeed, squaredInputs);
}

// ---- END DIFFERENTIAL_DRIVE METHODS ----


/**
 * Attempt to drive in a straight line using the pre-defined bias variables.
 * This method could be used to conduct an automated test of the robot to help
 * determine the ideal bias values for a completely straight path.
 *
 * @param speed
 * the speed that the robot should drive forward at
 */
void PeanutChassis::driveStraight(double speed)
{
	TankDrive(speed + leftBias, speed + rightBias);
}

/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

