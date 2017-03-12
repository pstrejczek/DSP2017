// 
// 
// 

#include "ProximitySensorHandler.h"

ProximitySensorHandlerClass::ProximitySensorHandlerClass()
{
	pinMode(PROXIMITY_LEFT, INPUT);
	pinMode(PROXIMITY_RIGHT, INPUT);
}


ProximityState ProximitySensorHandlerClass::checkState()
{
	int leftState = digitalRead(PROXIMITY_LEFT);
	int rightState = digitalRead(PROXIMITY_RIGHT);

	if (leftState == 0 && rightState == 0) return BOTH;
	if (leftState == 0) return LEFT;
	if (rightState == 0) return RIGHT;
	return NONE;
}


ProximitySensorHandlerClass ProximitySensorHandler;

