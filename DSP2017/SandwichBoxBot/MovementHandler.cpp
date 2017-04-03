// 
// 
// 

#include "MovementHandler.h"

void MovementHandlerClass::init()
{
	_currentMovementMode = MODE_MANUAL;
}

CurrentMode MovementHandlerClass::getCurrentMode()
{
	return _currentMovementMode;
}


MovementHandlerClass MovementHandler;

