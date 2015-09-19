#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


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
int flywheelHighCommand = 80;
int flywheelLowCommand = 62;
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
int conveyorDir = -1;
bool conveyorSlow = false;
int conveyorSlowSpeed = 25;
int conveyorSpeed = 127;

void updateConveyor()
{
	motor[port9] = conveyorOn? (conveyorDir * (conveyorSlow? conveyorSlowSpeed : conveyorSpeed)) : 0;
}

void updateButtons()
{
	static bool flywheelHighBtn = false;
	if (vexRT(Btn5U) != flywheelHighBtn)
	{
		flywheelHighBtn = !flywheelHighBtn;
		if (vexRT(Btn5U))
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
	if (vexRT(Btn5D) != flywheelLowBtn)
	{
		flywheelLowBtn = !flywheelLowBtn;
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

	static bool flywheelIncreaseBtn = false;
	if (vexRT(Btn8U) != flywheelIncreaseBtn)
	{
		flywheelIncreaseBtn = !flywheelIncreaseBtn;
		if (vexRT(Btn8U))
		{
			switch (flywheelState)
			{
				case FLYWHEEL_LOW:
					flywheelLowCommand += 5;
					break;
				case FLYWHEEL_HIGH:
					flywheelHighCommand += 5;
			}
		}
	}

	static bool flywheelDecreaseBtn = false;
	if (vexRT(Btn8D) != flywheelDecreaseBtn)
	{
		flywheelDecreaseBtn = !flywheelDecreaseBtn;
		if (vexRT(Btn8D))
		{
			switch (flywheelState)
			{
				case FLYWHEEL_LOW:
					flywheelLowCommand -= 5;
					break;
				case FLYWHEEL_HIGH:
					flywheelHighCommand -= 5;
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

task autonomousSequence()
{
	conveyorOn = false;

	flywheelState = FLYWHEEL_LOW;

	wait1Msec(3000);

	conveyorOn = true;
	conveyorSlow = true;
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

	startTask(autonomousSequence);
	while (true)
	{
		updateFlywheel();
		updateConveyor();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////




task usercontrol()
{
	// User control code here, inside the loop

	conveyorSlow = false;
	while (true)
	{
		if (vexRT(Btn7U))
		{
			startTask(autonomous);
			wait1Msec(15000);
			conveyorSlow = false;
		}
		//flywheelOn = (bool)vexRT(Btn5U);
		//conveyorOn = (bool)vexRT(Btn6U);
		updateButtons();
		updateDrive();
		updateFlywheel();
		updateConveyor();
		//updateButtons();
	}
}
