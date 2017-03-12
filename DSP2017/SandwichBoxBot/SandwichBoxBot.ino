/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14

DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;

DriveDirection driveDirection;
ProximityState proximityState;

void setup() {
	pinMode(BUZZER, OUTPUT);
	
	driveDirection = FORWARD;
	proximityState = NONE;
	
}


void loop() {
	proximityState = ProximitySensors.checkState();
	
	if(proximityState == BOTH || proximityState == LEFT || proximityState == RIGHT)
	{
		digitalWrite(BUZZER, HIGH);
		Drive.stopDrive();
		
		delay(2000);

		digitalWrite(BUZZER, LOW);
		if (driveDirection == FORWARD) driveDirection = BACKWARD;
		else driveDirection = FORWARD;
	}
	else
	{
		digitalWrite(BUZZER, LOW);
		Drive.startDrive(driveDirection);
	}
}

