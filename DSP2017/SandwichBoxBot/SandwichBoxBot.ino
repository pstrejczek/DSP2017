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

const char* ssid = "***";
const char* password = "***";

void prepareOTA()
{
	ArduinoOTA.onStart([]()
	{
		Serial.println("Transfer start ");
	});


	ArduinoOTA.onEnd([]()
	{
		Serial.println("\nTransfer end");
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Transfer progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	ArduinoOTA.begin();
}

void setup() {
	// Setup parameters for OTA and serial debug
	Serial.begin(115200);
	
	WebHandler.init();
	
	prepareOTA();
	
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	// Setup robot
	pinMode(BUZZER, OUTPUT);
	proximityState = NONE;
}




void loop() {
	ArduinoOTA.handle(); // handle the OTA request

	proximityState = ProximitySensors.checkState();
	
	if(proximityState == BOTH || proximityState == LEFT || proximityState == RIGHT)
	{
		digitalWrite(BUZZER, HIGH);
		Drive.stopDrive();
		
		delay(2000);

		digitalWrite(BUZZER, LOW);
		Drive.reverseDirection();
	}
	else
	{
		digitalWrite(BUZZER, LOW);
		Drive.startDrive();
	}
}



