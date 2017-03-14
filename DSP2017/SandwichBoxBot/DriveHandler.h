// Drive.h

#ifndef _DRIVE_h
#define _DRIVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DRIVE_AIA 12
#define DRIVE_AIB 13
#define DRIVE_BIA 5
#define DRIVE_BIB 4

typedef enum DriveDirection {
	FORWARD,
	BACKWARD
};

class DriveHandlerClass
{
 private:
	DriveDirection direction;
 protected:


 public:
	 DriveHandlerClass();
	void stopDrive();
	void startDrive();
	void reverseDirection();
};

extern DriveHandlerClass DriveHandler;

#endif

