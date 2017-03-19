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

typedef enum WebResponderType
{
	ACCESS_POINT,
	STATION
};


class WebHandlerClass
{
private:
	String _ssid;
	String _password;
	String _hostName;
	WebResponderType _currentResponderType;
	
	bool isWiFiConnected();
	
	void initializeWeb(WebResponderType type);
	void setupAccessPoint();
	
	void webHandleRoot();
	void webHandleChangeCredentials();
	void webHandleClearEeprom();
	void webHandleGetIp();
	void webHandleCurrentMode();
	void webHandleGetCurrentNetwork();
	void webHandleNotFound();
	
	String getAvaiableNetworks();
	String getIpAsString();
 protected:


 public:
	bool isInitialized;
	bool processRequest();
	void init();
	WebResponderType getCurrentResponderType();
};

extern WebHandlerClass WebHandler;

#endif

