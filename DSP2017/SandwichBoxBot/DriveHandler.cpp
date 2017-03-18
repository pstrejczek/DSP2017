#include "DriveHandler.h"

void DriveHandlerClass::manualForward()
{
	analogWrite(DRIVE_AIA, FULLSPEED);
	analogWrite(DRIVE_AIB, STOP);
	analogWrite(DRIVE_BIA, FULLSPEED);
	analogWrite(DRIVE_BIB, STOP);
}

void DriveHandlerClass::manualBackward()
{
	analogWrite(DRIVE_AIA, STOP);
	analogWrite(DRIVE_AIB, FULLSPEED);
	analogWrite(DRIVE_BIA, STOP);
	analogWrite(DRIVE_BIB, FULLSPEED);
}

void DriveHandlerClass::manualLeft()
{
	analogWrite(DRIVE_AIA, STOP);
	analogWrite(DRIVE_AIB, FULLSPEED);
	analogWrite(DRIVE_BIA, FULLSPEED);
	analogWrite(DRIVE_BIB, STOP);
}

void DriveHandlerClass::manualRight()
{
	analogWrite(DRIVE_AIA, FULLSPEED);
	analogWrite(DRIVE_AIB, STOP);
	analogWrite(DRIVE_BIA, STOP);
	analogWrite(DRIVE_BIB, FULLSPEED);
}

DriveHandlerClass::DriveHandlerClass()
{
	direction = FORWARD;
	pinMode(DRIVE_AIA, OUTPUT);
	pinMode(DRIVE_AIB, OUTPUT);
	pinMode(DRIVE_BIA, OUTPUT);
	pinMode(DRIVE_BIB, OUTPUT);
}

void DriveHandlerClass::stopDrive()
{
	analogWrite(DRIVE_AIA, 0);
	analogWrite(DRIVE_AIB, 0);
	analogWrite(DRIVE_BIA, 0);
	analogWrite(DRIVE_BIB, 0);
}

void DriveHandlerClass::startDrive()
{
	switch (direction)
	{
	case FORWARD:
		analogWrite(DRIVE_AIA, 1023);
		analogWrite(DRIVE_AIB, 0);
		analogWrite(DRIVE_BIA, 1023);
		analogWrite(DRIVE_BIB, 0);
		break;
	case BACKWARD:
		analogWrite(DRIVE_AIA, 0);
		analogWrite(DRIVE_AIB, 1023);
		analogWrite(DRIVE_BIA, 0);
		analogWrite(DRIVE_BIB, 1023);
		break;
	default:
		break;
	}
}

void DriveHandlerClass::reverseDirection()
{
	if (direction == FORWARD) direction = BACKWARD;
	else direction = FORWARD;
}

void DriveHandlerClass::manualSteering(ManualCommand manualCommand)
{
	switch(manualCommand)
	{
	case MANUAL_FORWARD:
		manualForward();
		break;
	case MANUAL_BACKWARD:
		manualBackward();
		break;
	case MANUAL_RIGHT:
			manualRight();
			break;
	case MANUAL_LEFT:
		manualLeft();
		break;
	default:
		stopDrive();
		break;
	}
	delay(1000);
	stopDrive();
}


DriveHandlerClass DriveHandler;

