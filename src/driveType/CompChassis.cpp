#include <driveType/CompChassis.h>
#include "WPILib.h"

CompChassis::CompChassis()
{
	// Though this shows an error, it will still build
	leftTalonMaster = new WPI_TalonSRX(0);
	rightTalonMaster = new WPI_TalonSRX(1);

	drive = new DifferentialDrive(*leftTalonMaster, *rightTalonMaster);

	joystick = new Joystick(0);

}

CompChassis::~CompChassis()
{
	// De-allocate pointers
	delete leftTalonMaster;
	delete rightTalonMaster;
}

void CompChassis::autonomousPeriodic()
{

}

void CompChassis::teleopPeriodic()
{
	char str[80];
	sprintf(str, "Message from Competition Chassis");
	DriverStation::ReportError(str);
}


/**
 * Arcade drive method for differential drive platform. The calculated values
 * will be squared to decrease sensitivity at low speeds. This method
 * will use the motors and configurations required by the Competition Chassis.
 *
 * @param xSpeed
 * the robot's speed along the X axis [-1.0..1.0]. Forward is positive.
 * @param zRotation
 * the robot's rotation rate around the Z axis [-1.0..1.0]. Clockwise is positive.
 * @param squaredInputs
 * default value is true, which will help limit sensitivity at slow speeds.
 */
void CompChassis::ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs)
{
	drive->ArcadeDrive(xSpeed, zRotation, squaredInputs);
}

/**
 * The rotation argument controls the curvature of the robot's path rather
 * than its rate of heading change. This makes the robot more controllable
 * at high speeds. Also handles the robot's quick turn functionality
 * - "quick turn" overrides constant-curvature turning for turn-in-place
 * maneuvers. This will use the configuration for the Competition Chassis.
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
void CompChassis::CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn)
{
	drive->CurvatureDrive(xSpeed, zRotation, isQuickTurn);
}

/**
 * Drive that explicitly takes the power of the left and right sides of the
 * robot. squaredInputs will help reduce sensitivity if set to true. This
 * method will use the configuration for the Competition Chassis.
 *
 * @param leftSpeed
 * the speed of the left side of the robot
 * @param rightSpeed
 * the speed of the right side of the robot
 * @param squredInputs
 * defalut value is true. This will control how sensitive the robot is
 */
void CompChassis::TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs)
{
	drive->TankDrive(leftSpeed, rightSpeed, squaredInputs);
}

/**
 * Attempt to drive in a straight line using the pre-defined bias variables.
 * This method could be used to conduct an automated test of the robot to help
 * determine the ideal bias values for a completely straight path.
 *
 * @param speed
 * the speed that the robot should drive forward at
 */
void CompChassis::driveStraight(double speed)
{
	TankDrive(speed + leftBias, speed + rightBias);
}

/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

