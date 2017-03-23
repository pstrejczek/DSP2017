/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#include "UdpCommHandler.h"
#include "WiFiHandler.h"
#include "WebHandler.h"
#include "EepromDataHandler.h"
#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14
#define LOCAL_UDP_PORT 1234;

enum CurrentMode
{
	MODE_MANUAL,
	MODE_AUTO
};

DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;

EepromDataHandlerClass EepromWebConfigHandler;
WiFiHandlerClass WiFiHandler;
WebHandlerClass WebHandler;
UdpCommHandlerClass UdpCommHandler;

ProximityState proximityState;
CurrentMode currentMode;

void setup() {

	Serial.begin(115200);
	
	currentMode = MODE_MANUAL;
	
	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work
	EepromWebConfigHandler.readEepromWiFiParameters();
	WiFiHandler.init(EepromWebConfigHandler.getSsid(), EepromWebConfigHandler.getPassword());

	// Initialize WebServer
	WebHandler.init(EepromWebConfigHandler, WiFiHandler);
		
	// Initialize UdpCommHandler
	UdpCommHandler.init(1234);

	// Setup robot
	pinMode(BUZZER, OUTPUT);
	proximityState = NONE;
}

void loop() 
{
	Command currentCommand = C_NONE;
	// Web and OTA
	if (WebHandler.isInitialized)
	{
		// Process web requests
		WebHandler.processRequest();
	}
	
	// UDP Command Server
	if (UdpCommHandler.isInitialized)
	{
		currentCommand = UdpCommHandler.processCommandRequest();
		if (currentCommand == C_PACKET_ERROR) Serial.println("UDP packet data error");
	}

	// Process Command
	switch(currentCommand)
	{
	case C_AUTO: currentMode = MODE_AUTO; Serial.println("SET MODE AUTO"); break;
	case C_MANUAL: currentMode = MODE_MANUAL; Serial.println("SET MODE MANUAL"); break;
	case C_FORWARD: Drive.manualSteering(MANUAL_FORWARD); break;
	case C_BACKWARD: Drive.manualSteering(MANUAL_BACKWARD); break;
	case C_LEFT: Drive.manualSteering(MANUAL_LEFT); break;
	case C_RIGHT: Drive.manualSteering(MANUAL_RIGHT); break;
	default: break;
	}
	
	// Robot logic
	robotLogic();
}

void robotLogic()
{
	proximityState = ProximitySensors.checkState();

	if (proximityState == BOTH || proximityState == LEFT || proximityState == RIGHT)
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



