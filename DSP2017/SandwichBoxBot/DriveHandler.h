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

#define FULLSPEED 1023
#define HALFSPEED 512
#define STOP 0

typedef enum DriveDirection {
	FORWARD,
	BACKWARD
};

typedef enum ManualCommand {
	MANUAL_FORWARD,
	MANUAL_BACKWARD,
	MANUAL_RIGHT,
	MANUAL_LEFT
};

class DriveHandlerClass
{
 private:
	DriveDirection direction;
	void manualForward();
	void manualBackward();
	void manualLeft();
	void manualRight();
 protected:


 public:
	 DriveHandlerClass();
	void stopDrive();
	void startDrive();
	void reverseDirection();
	void manualSteering(ManualCommand manualCommand);
};

extern DriveHandlerClass DriveHandler;

#endif

