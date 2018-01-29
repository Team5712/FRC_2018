#include "driveType/CompChassis.h"
#include "WPILib.h"

/**
 * Initalize the objects needed for this Chassis type here. Due note that
 * these operations are called in RobotInit(), and will take effect as
 * soon as the robot is powered on. For exmaple, a timer started in
 * the constructor will continue to run and may return unexpected
 * or incorrect values. Instead, start the timer in the designated
 * init() method (autonomousInit() or teleopInit()).
 */
CompChassis::CompChassis()
{
	// Though this shows an error, it will still build
	leftTalonMaster = new WPI_TalonSRX(2);
	rightTalonMaster = new WPI_TalonSRX(1);

	drive = new DifferentialDrive(*leftTalonMaster, *rightTalonMaster);

	joystick = new Joystick(0);

	leftTalonMaster->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	rightTalonMaster->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

}

/**
 * Delete and dereference pointers here. Avoid deleting pointers
 * that originate from BaseDrive.hpp as it will delete the pointers in its
 * deconstructor.
 */
CompChassis::~CompChassis()
{
	// De-allocate pointers
	delete leftTalonMaster;
	delete rightTalonMaster;
}


// ---- ROBOT.CPP METHODS ----

/**
 * Called by Robot.cpp when the Autonomous mode was initalized. Operations
 * may include resetting the value of encoders, starting timers, or zeroing
 * in the gyro. Overrides from BaseDrive.hpp
 */
void CompChassis::autonomousInit()
{
	leftTalonMaster->SetSelectedSensorPosition(0, 0, 10);
	rightTalonMaster->SetSelectedSensorPosition(0, 0, 10);

}

/**
 * Called by Robot.cpp during autonomous. This is a looped function and will
 * be caused every cycle of the Robot. This should include all of the
 * operations for autonomous. Overrides from BaseDrive.hpp
 */
void CompChassis::autonomousPeriodic()
{
	char str[80];
	sprintf(str, "L encoder = %d", leftTalonMaster->GetSelectedSensorPosition(0));
	DriverStation::ReportError(str);
}

/**
 * Method that is called at the beginning of TeleOp. It will be called once
 * and should initalize and set the defaults for the Robot. Overrides
 * from BaseDrive.hpp
 */
void CompChassis::teleopInit()
{
	leftTalonMaster->SetSelectedSensorPosition(0, 0, 10);
	rightTalonMaster->SetSelectedSensorPosition(0, 0, 10);

}

/**
 * Called in a loop from Robot.cpp. It should include general operations for
 * driving the robot, such as joystick input, motor output, and reading
 * various sensor values. Overrides from BaseDrive.hpp
 */
void CompChassis::teleopPeriodic()
{
//	 This works:
//	char str[80];
//	sprintf(str, "Message from Competition Chassis");
//	DriverStation::ReportError(str);

	ArcadeDrive(getJoystickValue(1), getJoystickValue(0));
}

// ---- END ROBOT.CPP METHODS ----
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

