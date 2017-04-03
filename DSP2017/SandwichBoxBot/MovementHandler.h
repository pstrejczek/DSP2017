// MovementHandler.h

#ifndef _MOVEMENTHANDLER_h
#define _MOVEMENTHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef enum CurrentMode
{
	MODE_MANUAL,
	MODE_AUTO
};

class MovementHandlerClass
{
private:
	CurrentMode _currentMovementMode;
 protected:


 public:
	void init();
	CurrentMode getCurrentMode();
};

extern MovementHandlerClass MovementHandler;

#endif

