// ProximitySensorHandler.h

#ifndef _PROXIMITYSENSORHANDLER_h
#define _PROXIMITYSENSORHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define PROXIMITY_LEFT 2
#define PROXIMITY_RIGHT 16

typedef enum ProximityState{
	LEFT,
	RIGHT,
	BOTH,
	NONE
};


class ProximitySensorHandlerClass
{
 protected:


 public:
	 ProximitySensorHandlerClass();
	 ProximityState checkState();
};

extern ProximitySensorHandlerClass ProximitySensorHandler;

#endif

