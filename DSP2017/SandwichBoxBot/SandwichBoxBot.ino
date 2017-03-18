/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#include "WebHandler.h"
#include "EepromDataHandler.h"



#include<ArduinoOTA.h>

#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14

DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;
WebHandlerClass WebHandler;

ProximityState proximityState;

void setup() {

	Serial.begin(115200);
	
	WebHandler.init();
		
	// Setup robot
	pinMode(BUZZER, OUTPUT);
	proximityState = NONE;
}

void loop() 
{
	// Web and OTA
	if (WebHandler.isInitialized)
	{
		// Process web requests
		WebHandler.processRequest();
	}
	
	// Robot logic
	proximityState = ProximitySensors.checkState();
	
	if(proximityState == BOTH || proximityState == LEFT || proximityState == RIGHT)
	{
		digitalWrite(BUZZER, HIGH);
		Drive.stopDrive();
		
		//delay(2000);

		digitalWrite(BUZZER, LOW);
		Drive.reverseDirection();
	}
	else
	{
		digitalWrite(BUZZER, LOW);
		Drive.startDrive();
	}
}



