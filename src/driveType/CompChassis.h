#ifndef SRC_DRIVETYPE_COMPCHASSIS_H_
#define SRC_DRIVETYPE_COMPCHASSIS_H_

#include <driveType/BaseDrive.hpp>
#include "../Constants.h"
#include "ctre/Phoenix.h"
#include <math.h>


//using namespace CTRE::Phoenix::MotorControl::CAN;

class CompChassis
{
public:

	CompChassis(Spark *l);
	virtual ~CompChassis();

	// Override DifferentialDrive methods from the base drive class
	// ---- DIFFERENTIAL_DRIVE METHODS ----
	void ArcadeDrive(double xSpeed, double zRotation, bool squaredInputs = true);
	void CurvatureDrive(double xSpeed, double zRotation, bool isQuickTurn);
	void TankDrive(double leftSpeed, double rightSpeed, bool squaredInputs = true);

	void autonomousInit();
	void autonomousPeriodic();
	void teleopInit();
	void teleopPeriodic();
	void resetEncoders();
	void initMotionMagic();

	void updateLift();
	void updateVisionButtons();
	void updateBlinkin();
	void updateGrippers();
	void handleInput();
	void updateShifter();
	void updateIntake();
	void updatePneumatics();
	void updateClimber();

	void setGrippers(double power);
	void setLeftRight(double, double);
	double getCurrentLeft();
	double getCurrentRight();


	struct MotionMagic {
		const float TIMEOUT = 0.005;
		const float ACCEL = 3000;
		const float VEL = 8000;
		const float TICKS_PER_FOOT = Constants::RATIO * 12;
		const float MAX_TICKS_PER_SECOND = 8000;
		const float l_P = 0.0;
		const float l_I = 0.0;
		const float l_D = 0.0;
		const float l_F = 0.70;

		const float r_P = 0.0;
		const float r_I = 0.0;
		const float r_D = 0.0;
		const float r_F = 0.70;
		const float DISTANCE = 5;
	} mm;


	bool isAtPIDPosition(double distance);

	double current_led = 0;
	Solenoid *shifter;



	DifferentialDrive *drive;

	WPI_TalonSRX *l_master;
	WPI_TalonSRX *r_master;
	WPI_TalonSRX *lift_master;

	std::shared_ptr<NetworkTable> table;
	float left_command = 0;
	float right_command = 0;

	Spark *led;
	AHRS *gyro;

	double r_ratio = 1.05;

	bool liftFloor();
	bool liftSwitch();
	bool liftScale();

	double getLiftJoystick();

	// inputs
	bool isB = false;
	bool isY = false;
	bool isA = false;
	bool isX = false;

	bool isLeftMiddleButton = false;
	bool isRightMiddleButton = false;

	bool isRightTrigger = false;
	bool isLeftTrigger = false;

	bool isRightBumper = false;
	bool isLeftBumper = false;

	bool isStartButton = false;

	bool isLeftStickButton = false;
	bool isRightStickButton = false;

	bool isLeftDriveTrigger = false;
	bool isRightDriveTrigger = false;

	float leftJoystickAxis = 0.0;
	float rightJoystickAxis = 0.0;


	// ---- END DIFFERENTIAL_DRIVE METHODS ----

//	// DEPRECATED
//	// ---- ROBOT.CPP METHODS ----
//	void autonomousInit() override;
//	void autonomousPeriodic() override;
//	void teleopInit() override;
//	void teleopPeriodic() override;
//	// ---- END ROBOT.CPP METHODS ----

	// Overritten from BaseDrive.hpp
	void driveStraight(double speed);
	int getLeftValue();
	int getRightValue();

	Timer match_timer;
	bool isClimbing = false;

	Timer intakeTimer;
	bool isIntakeTimerStart = false;
	double intakeFinishDuration = 3.0;
	// this is set to true after we are done taking in cube + intakeFinishDUration
	bool isDoneIntaking = false;

	bool isLedTimerStart = false;
	Timer LedTimer;
	double led_swap_interval = 3.0;

	//
	bool isCylinderTimerStart = false;
	Timer cylinder_timer_down;
	double cylinder_down_wait = 1.0;


	// timer for shooting the cube after a delay
	bool isShoot_timer_start = false;
	Timer shoot_timer;
	double shoot_cube_duration = 1.0;
	bool shootCube();


	//bool succCube();

	cs::UsbCamera camera;

	DigitalInput *limitSwitch;


private:
	// Probably have multiple motors, so declare master / slaves here

	//std::shared_ptr<NetworkTable> table;


	WPI_VictorSPX *l_slave1;
	WPI_VictorSPX *l_slave2;
	WPI_VictorSPX *l_slave3;

	WPI_VictorSPX *r_slave1;
	WPI_VictorSPX *r_slave2;
	WPI_VictorSPX *r_slave3;

	WPI_VictorSPX *l_grip;
	WPI_VictorSPX *r_grip;

	AnalogPotentiometer *pot;



	Compressor *compressor;

	DoubleSolenoid *cylinder;


	DoubleSolenoid *climber;
	bool hasCube;

	bool isGrip_timerStart = false;
	Timer grip_timer;

	bool grip_isLeft = true;
	double pulse_interval = 0.3;
	double offPulsePower = 0.30;
	double onPulsePower = 1.0;

	bool isUp = false;

	bool lift_isFloor = false;
	bool lift_isSwitch = false;
	bool lift_isScale = false;

	double liftSustainPower = 0.115;

	// 36 is max
	double lift_potScale = 45;
	double lift_potSwitch = 320;
	double lift_potFloor = 400;

	Joystick *l_joystick;
	Joystick *r_joystick;
	Joystick *joystick_lift;



	float led_values[100];


	bool forward = true;
	bool in = true;
	bool up = true;



};

#endif /* SRC_DRIVETYPE_COMPCHASSIS_H_ */
