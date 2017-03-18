// WebHandler.h

#ifndef _WEBHANDLER_h
#define _WEBHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include<ESP8266WiFi.h>
#include<ESP8266mDNS.h>
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
	WebResponderType _currentResponderType;
	
	bool isWiFiConnected();
	
	void initializeWeb(WebResponderType type);
	void setupAccessPoint();
	void processAccessPointWebRequest(String req, WiFiClient client);
	void processStationWebRequest(String req, WiFiClient client);
	bool processGlobalRequests(String req, WiFiClient client);
	String getAvaiableNetworks();
	String getIpAsString();
 protected:


 public:
	bool isInitialized;
	bool checkMDns();
	void init();
	WebResponderType getCurrentResponderType();
};

extern WebHandlerClass WebHandler;

#endif
