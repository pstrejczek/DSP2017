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

class WebHandlerClass
{
private:
	String _ssid;
	String _password;
	bool isWiFiConnected();
	int checkMDns(int type);
	void initializeWeb(int type);
	void setupAp();
 protected:


 public:
	void init();
	
};

extern WebHandlerClass WebHandler;

#endif

