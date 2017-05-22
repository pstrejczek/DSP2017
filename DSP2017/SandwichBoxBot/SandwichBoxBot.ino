/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe³ Strejczek
*/

#include "MovementHandler.h"
#include "UdpCommHandler.h"
#include "WiFiHandler.h"
#include "WebHandler.h"
#include "EepromDataHandler.h"
#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14
#define LOCAL_UDP_PORT 1234;


DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;

MovementHandlerClass Movement;

EepromDataHandlerClass EepromWebConfigHandler;
WiFiHandlerClass WiFiHandler;
WebHandlerClass WebHandler;
UdpCommHandlerClass UdpCommHandler;

ProximityState proximityState;
CurrentMode currentMode;

Command currentManualCommand = C_NONE;
int test = 0;

void setup() {

	Serial.begin(115200);
	
	Movement.init();
	
	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work
	EepromWebConfigHandler.readEepromWiFiParameters();
	WiFiHandler.init(EepromWebConfigHandler.getSsid(), EepromWebConfigHandler.getPassword());

	// Initialize WebServer
	WebHandler.init(EepromWebConfigHandler, WiFiHandler, Movement);
		
	// Initialize UdpCommHandler
	UdpCommHandler.init(1234);

	// Setup robot
	pinMode(BUZZER, OUTPUT);
	proximityState = NONE;
}

void loop()
{
	// Web and OTA
	if (WebHandler.isInitialized)
	{
		WebHandler.processRequest();
	}

	// UDP Command Server
	
	if (UdpCommHandler.isInitialized)
	{
		currentManualCommand = UdpCommHandler.processCommandRequest();
		if (currentManualCommand == C_PACKET_ERROR) Serial.println("UDP packet data error");
	}

	// Process Command
	switch(currentManualCommand)
	{
		case C_AUTO: currentMode = MODE_AUTO; Movement.SetCurrentMode(MODE_AUTO); break;
		case C_MANUAL: currentMode = MODE_MANUAL; Movement.SetCurrentMode(MODE_MANUAL); break;
		case C_FORWARD: Drive.manualSteering(MANUAL_FORWARD); break;
		case C_BACKWARD: Drive.manualSteering(MANUAL_BACKWARD); break;
		case C_LEFT: Drive.manualSteering(MANUAL_LEFT); break;
		case C_RIGHT: Drive.manualSteering(MANUAL_RIGHT); break;
		case C_STOP: Drive.stopDrive(); break;
		default: break;
	}
	
	// Robot logic
	if(Movement.getCurrentMode() == MODE_AUTO) automaticRobotLogic();
}

void automaticRobotLogic()
{
	proximityState = ProximitySensors.checkState();

	if (proximityState == BOTH || proximityState == LEFT)
	{
		digitalWrite(BUZZER, HIGH);
		Drive.stopDrive();
		delay(500);
		digitalWrite(BUZZER, LOW);
		Drive.manualSteering(MANUAL_BACKWARD);
		delay(500);
		Drive.manualSteering(MANUAL_RIGHT);
		delay(500);
		Drive.stopDrive();
	}
	else if (proximityState == RIGHT)
	{
		digitalWrite(BUZZER, HIGH);
		Drive.stopDrive();
		delay(500);
		digitalWrite(BUZZER, LOW);
		Drive.manualSteering(MANUAL_BACKWARD);
		delay(500);
		Drive.manualSteering(MANUAL_LEFT);
		delay(500);
		Drive.stopDrive();
	}
	else
	{
		digitalWrite(BUZZER, LOW);
		Drive.manualSteering(MANUAL_FORWARD);
	}
}



