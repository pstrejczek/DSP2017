// WebHandler.h

#ifndef _WEBHANDLER_h
#define _WEBHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

#include<WiFiUdp.h>
#include "EepromDataHandler.h"
#include "WiFiHandler.h"
#include "MovementHandler.h"

class WebHandlerClass
{
private:
	String _ssid;
	String _password;

	EepromDataHandlerClass _eeprom;
	WiFiHandlerClass _wifi;
	MovementHandlerClass _movement;

	void initializeWeb();
	
	void webHandleRoot();
	void webHandleChangeCredentials();
	void webHandleClearEeprom();
	void webHandleGetIp();
	void webHandleCurrentMode();
	void webHandleCurrentRobotMode();
	void webHandleGetCurrentNetwork();
	void webHandleAvaiableNetworks();
	void webHandleNotFound();
	
	String getAvaiableNetworks();
 protected:


 public:
	bool isInitialized;
	bool processRequest();
	void init(EepromDataHandlerClass eeprom, WiFiHandlerClass wifi, MovementHandlerClass movement);
};

extern WebHandlerClass WebHandler;

#endif

