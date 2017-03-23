/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#include "WiFiHandler.h"
#include "WebHandler.h"
#include "EepromDataHandler.h"
#include "EepromDataHandler.h"
#include "WiFiHandler.h"



#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14


DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;

EepromDataHandlerClass EepromWebConfigHandler;
WiFiHandlerClass WiFiHandler;
WebHandlerClass WebHandler;

ProximityState proximityState;

void setup() {

	Serial.begin(115200);
	
	
	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work
	EepromWebConfigHandler.readEepromWiFiParameters();
	WiFiHandler.init(EepromWebConfigHandler.getSsid(), EepromWebConfigHandler.getPassword());

	// Initialize WebServer
	WebHandler.init(EepromWebConfigHandler, WiFiHandler);
		
	// Initialize CommUdpServer

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
	
	// UDP Command Server

	// Process Command

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



