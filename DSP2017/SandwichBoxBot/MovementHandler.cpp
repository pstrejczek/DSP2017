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

void MovementHandlerClass::SetCurrentMode(CurrentMode mode)
{
	_currentMovementMode = mode;
}


MovementHandlerClass MovementHandler;

