#include "driveType/PeanutChassis.h"
#include "WPILib.h"

/**
 * Initalize the objects needed for this Chassis type here. Due note that
 * these operations are called in RobotInit(), and will take effect as
 * soon as the robot is powered on. For exmaple, a timer started in
 * the constructor will continue to run and may return unexpected
 * or incorrect values. Instead, start the timer in the designated
 * init() method (autonomousInit() or teleopInit()).
 *
 * @param ticks - number of ticks/revolution on the wheel
 * @param wheel_radius - the radius of the wheels
 *
 */
PeanutChassis::PeanutChassis()
{
	leftTalon = new WPI_TalonSRX(2);
	rightTalon = new WPI_TalonSRX(1);

	drive = new DifferentialDrive(*leftTalon, *rightTalon);

	joystick = new Joystick(0);

	gyro = new AHRS(SPI::Port::kMXP, AHRS::kRawData, 200 /*samples/sec*/);

	timer = new Timer();

	pot = new AnalogPotentiometer(0, 1, 0);

	// Define the settings for the encoders
	leftTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	rightTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

}

/**
 * Delete and dereference pointers here. Avoid deleting pointers
 * that originate from BaseDrive.hpp as it will delete the pointers in its
 * deconstructor.
 */
PeanutChassis::~PeanutChassis()
{
	// De-reference pointers
	delete leftTalon;
	delete rightTalon;

	delete timer;
}

// ---- ROBOT.CPP METHODS ----

/**
 * Called by Robot.cpp when the Autonomous mode was initalized. Operations
 * may include resetting the value of encoders, starting timers, or zeroing
 * in the gyro. Overrides from BaseDrive.hpp
 */
void PeanutChassis::autonomousInit()
{
	gyro->ZeroYaw();
	leftTalon->SetSelectedSensorPosition(0, 0, 10);
	rightTalon->SetSelectedSensorPosition(0, 0, 10);

	timer->Reset();
	timer->Start();

}

/**
 * Called by Robot.cpp during autonomous. This is a looped function and will
 * be caused every cycle of the Robot. This should include all of the
 * operations for autonomous. Overrides from BaseDrive.hpp
 */
void PeanutChassis::autonomousPeriodic()
{
	//int initialVal = leftTalon->GetSelectedSensorPosition(0);
	leftTraveled = leftTalon->GetSelectedSensorPosition(0) * RATIO;
	rightTraveled = rightTalon->GetSelectedSensorPosition(0) * RATIO;

//	char str2[80];
//	sprintf(str2, "value of leftTraveled: %d", leftTraveled);
//	DriverStation::ReportError(str2);
	if(abs(leftTraveled) < distance)
	{
		TankDrive(0.3, 0.3);
	} else
	{
		TankDrive(0.0, 0.0);
	}

	// NOTE: The encoder values return as an INTEGER, not a double
//	char str[80];
//	sprintf(str, "L = %d, R = %d", leftTalon->GetSelectedSensorPosition(0), rightTalon->GetSelectedSensorPosition(0));
//	DriverStation::ReportError(str);
//	sprintf(str, "gyro: %f", gyro->GetYaw());
//	DriverStation::ReportError(str);

}

/**
 * Method that is called at the beginning of TeleOp. It will be called once
 * and should initalize and set the defaults for the Robot. Overrides
 * from BaseDrive.hpp
 */
void PeanutChassis::teleopInit()
{
	leftTalon->SetSelectedSensorPosition(0, 0, 10);
	rightTalon->SetSelectedSensorPosition(0, 0, 10);
}

/**
 * Called in a loop from Robot.cpp. It should include general operations for
 * driving the robot, such as joystick input, motor output, and reading
 * various sensor values. Overrides from BaseDrive.hpp
 */
void PeanutChassis::teleopPeriodic()
{
//	char str[80];
//	sprintf(str, "Message from peanut");
//	DriverStation::ReportError("Message from Peanut chassis");

	ArcadeDrive(getJoystickValue(1), getJoystickValue(0));

	//double val = pot->Get();

	char str[128];
	//sprintf(str,"Snoopy is smokin pot at: %f", val);
	//DriverStation::ReportError(str);
	sprintf(str, "L = %d, R = %d", leftTalon->GetSelectedSensorPosition(0), rightTalon->GetSelectedSensorPosition(0));
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
	TankDrive(speed * leftBias, speed * rightBias);
}

/**
 * This will return the constant RATIO for the Peanut Chassis.
 * The ratio can be multiplied by the
 * desired distance (in inches) to return the number of ticks needed
 * to drive said distance.
 *
 * @return
 * A float value of the pre-defined ratio.
 */
float PeanutChassis::getRatio()
{
	return RATIO;
}

/*
 * These methods will use the default functionality defined in BaseDrive.hpp:
 *
 * double PeanutChassis::getJoystickValue(int axisNum);
 */

