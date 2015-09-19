#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes =  true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

typedef enum DriveStyle
{
	TANK,
	ARCADE
}
DriveStyle;

DriveStyle driveStyle = TANK;


void updateDrive()
{
		switch (driveStyle)
		{
			case TANK:
				// Right wheel
				motor[port7] = vexRT(Ch2);
				// Left wheel
				motor[port8] = vexRT(Ch3);
				break;
			case ARCADE:
				// Right wheel
				motor[port7] = vexRT(Ch2) - vexRT(Ch1);
				// Left wheel
				motor[port8] = vexRT(Ch2) + vexRT(Ch1);
				break;
		}
}

typedef enum FlywheelState
{
	FLYWHEEL_OFF,
	FLYWHEEL_1,
	FLYWHEEL_2,
	FLYWHEEL_3
}
FlywheelState;

static const int slewRate = 1;
static const int flywheel1TopCommand = 50
static const int flywheel1BototmCommand = 50
static const int flywheel2TopCommand = 95;
static const int flywheel2BototmCommand = 95;
static const int flywheel3TopCommand = 100;
static const int flywheel3BottomCommand = 100;
float flywheelCommandTop = 0;
float flywheelCommandBottom = 0;

FlywheelState flywheelState = FLYWHEEL_OFF;

void updateFlywheel()
{
	int targetTop;
	int targetBottom;
	switch (flywheelState)
	{
		case FLYWHEEL_OFF:
			targetTop = 0;
			targetBottom = 0;
			break;
		case FLYWHEEL_1
			targetTop = flywheel2TopCommand;
			targetBottom = flywheel2BottomCommand;
			break;
		case FLYWHEEL_2
			targetTop = flywheelHighTopCommand;
			targetBottom = flywheelHighBottomCommand;
	}
	if (flywheelCommandTop > targetTop)
	{
		flywheelCommandTop -= slewRate;
	}
	else if (flywheelCommandTop < targetTop)
	{
		flywheelCommandTop += slewRate;
	}
	if (flywheelCommandBottom > targetBottom)
	{
		flywheelCommandBottom -= slewRate;
	}
	else if (flywheelCommandBottom < targetBottom)
	{
		flywheelCommandBottom += slewRate;
	}
	int cmdTop = (int)flywheelCommandTop;
	int cmdBottom = (int)flywheelCommandBottom;
	motor[port2] = cmdTop;
	motor[port3] = cmdTop;
	motor[port4] = cmdTop;
	motor[port5] = cmdBottom;
	motor[port6] = cmdBottom;
}

bool conveyorOn = false;
int conveyorDir = 1;

void updateConveyor()
{
	motor[port9] = conveyorOn? (conveyorDir * 127) : 0;
}

void updateButtons()
{
	static bool flywheelOnBtn = false;
	if (vexRT(Btn5U) != flywheelOnBtn)
	{
		flywheelOnBtn = !flywheelOnBtn;
		if (vexRT(Btn5U))
		{
			flywheelOn !flywheelOn;
		}
	}
	static bool flywheelSpeedBtn = false;
	if (vexRT(Btn5D) != flywheelSpeedBtn)
	{
		flywheelSpeedBtn = !flywheelSpeedBtn;
		if (vexRT(Btn5D))
		{
			switch (flywheelState)
			{
				case FLYWHEEL_OFF:
					flywheelState = FLYWHEEL_LOW;
					break;
				case FLYWHEEL_LOW:
					flywheelState = FLYWHEEL_OFF;
					break;
				case FLYWHEEL_HIGH:
					flywheelState = FLYWHEEL_LOW;
					break;
			}
		}
	}

	static bool conveyorBtn = false;
	if (vexRT(Btn6U) != conveyorBtn)
	{
		conveyorBtn = !conveyorBtn;
		if (vexRT(Btn6U))
		{
			conveyorOn = !conveyorOn;
		}
	}

	static bool conveyorDirBtn = false;
	if (vexRT(Btn6D) != conveyorDirBtn)
	{
		conveyorDirBtn = !conveyorDirBtn;
		if (vexRT(Btn6D))
		{
			conveyorDir *= -1;
		}
	}

	static bool driveStyleBtn = false;
	if (vexRT(Btn7D) != driveStyleBtn)
	{
		driveStyleBtn = !driveStyleBtn;
		if (vexRT(Btn7D))
		{
			switch (driveStyle)
			{
				case ARCADE:
					driveStyle = TANK;
					break;
				case TANK:
					driveStyle = ARCADE;
					break;
			}
		}
	}


}


task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{
		//flywheelOn = (bool)vexRT(Btn5U);
		//conveyorOn = (bool)vexRT(Btn6U);
		updateButtons();
		updateDrive();
		updateFlywheel();
		updateConveyor();
		//updateButtons();
	}
}
