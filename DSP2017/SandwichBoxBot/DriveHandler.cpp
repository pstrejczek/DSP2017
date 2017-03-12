#include "DriveHandler.h"

DriveHandlerClass::DriveHandlerClass()
{
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

void DriveHandlerClass::startDrive(DriveDirection dState)
{
	switch (dState)
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


DriveHandlerClass DriveHandler;

