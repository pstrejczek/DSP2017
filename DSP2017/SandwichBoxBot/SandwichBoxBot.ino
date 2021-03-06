/*
 Name:		SandwichBoxBot.ino
 Created:	2/28/2017 7:02:30 PM
 Author:	Pawe� Strejczek
*/

#include "UdpCommHandler.h"
#include "WiFiHandler.h"
#include "WebHandler.h"
#include "EepromDataHandler.h"
#include "ProximitySensorHandler.h"
#include "DriveHandler.h"

#define BUZZER 14
#define LOCAL_UDP_PORT 1234;

#define INPUTFILTER 100;

DriveHandlerClass Drive;
ProximitySensorHandlerClass ProximitySensors;

EepromDataHandlerClass EepromWebConfigHandler;
WiFiHandlerClass WiFiHandler;
WebHandlerClass WebHandler;
UdpCommHandlerClass UdpCommHandler;

ProximityState proximityState;

Command currentManualCommand = C_NONE;
int leftCounter = 0;
int rightCounter = 0;

int inputFilter = 100;

void setup() {

	Serial.begin(115200);
	
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
	case C_AUTO: Drive.stopDrive(); WebHandler.SetCurrentMode(MODE_AUTO); Serial.println("SET AUTO");  break;
	case C_MANUAL: Drive.stopDrive(); WebHandler.SetCurrentMode(MODE_MANUAL); Serial.println("SET MANUAL"); break;
	case C_FORWARD: Drive.manualSteering(MANUAL_FORWARD); break;
	case C_BACKWARD: Drive.manualSteering(MANUAL_BACKWARD); break;
	case C_LEFT: Drive.manualSteering(MANUAL_LEFT); break;
	case C_RIGHT: Drive.manualSteering(MANUAL_RIGHT); break;
	case C_STOP: Drive.stopDrive(); break;
	default: break;
	}
	
	// Robot logic
	if(WebHandler.getCurrentMode() == MODE_AUTO) automaticRobotLogic();
}

void automaticRobotLogic()
{
	proximityState = ProximitySensors.checkState();

	if (proximityState == LEFT)
	{
		leftCounter++;
		if(leftCounter > inputFilter)
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
			leftCounter = 0;
		}
	}
	else leftCounter = 0;

	if (proximityState == RIGHT)
	{
		rightCounter++;
		if (rightCounter > inputFilter)
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
			rightCounter = 0;
		}
	}
	else rightCounter = 0;

	if (proximityState == BOTH)
	{
		leftCounter++;
		rightCounter++;
		if (leftCounter > inputFilter && rightCounter > inputFilter)
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
			leftCounter = 0;
			rightCounter = 0;
		}
	}

	if (leftCounter == 32766) leftCounter = 0;
	if (leftCounter == 32767) rightCounter = 0;

	
	if(leftCounter < inputFilter && rightCounter < inputFilter)
	{
		digitalWrite(BUZZER, LOW);
		Drive.manualSteering(MANUAL_FORWARD);
	}
}



