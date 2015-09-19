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
  bStopTasksBetweenModes = true;

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

typedef enum FlywheelState
{
	FLYWHEEL_OFF,
	FLYWHEEL_LOW,
	FLYWHEEL_HIGH
}
FlywheelState;


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

static const int slewRate = 1;
static const int flywheelHighCommand = 127;
static const int flywheelLowCommand = 13;
float flywheelCommand = 0;
//bool flywheelOn = false;
FlywheelState flywheelState = FLYWHEEL_OFF;

void updateFlywheel()
{
	int target;
	switch (flywheelState)
	{
		case FLYWHEEL_OFF:
			target = 0;
			break;
		case FLYWHEEL_LOW:
			target = flywheelLowCommand;
			break;
		case FLYWHEEL_HIGH:
			target = flywheelHighCommand;
	}
	if (flywheelCommand > target)
	{
		flywheelCommand -= slewRate;
	}
	else if (flywheelCommand < target)
	{
		flywheelCommand += slewRate;
	}
	int cmd = (int)flywheelCommand;
	motor[port2] = cmd;
	motor[port3] = cmd;
	motor[port4] = cmd;
	motor[port5] = cmd;
	motor[port6] = cmd;
}

bool conveyorOn = false;
int conveyorDir = 1;

void updateConveyor()
{
	motor[port9] = conveyorOn? (conveyorDir * 127) : 0;
}

void updateButtons()
{
	static bool flywheelHighBtn = false;
	if (vexRT(Btn6U) != flywheelHighBtn)
	{
		flywheelHighBtn = !flywheelHighBtn;
		if (vexRT(Btn6U))
		{
			switch (flywheelState)
			{
				case FLYWHEEL_OFF:
					flywheelState = FLYWHEEL_HIGH;
					break;
				case FLYWHEEL_LOW:
					flywheelState = FLYWHEEL_HIGH;
					break;
				case FLYWHEEL_HIGH:
					flywheelState = FLYWHEEL_OFF;
					break;
			}
		}
	}
	static bool flywheelLowBtn = false;
	if (vexRT(Btn6D) != flywheelLowBtn)
	{
		flywheelLowBtn = !flywheelLowBtn;
		if (vexRT(Btn6D))
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
					flywheelState = FLYWHEEL_HIGH;
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
