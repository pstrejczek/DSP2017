// WiFiHandler.h

#ifndef _WIFIHANDLER_h
#define _WIFIHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

typedef enum WiFiModes
{
	WFM_ACCESS_POINT,
	WFM_STATION
};

class WiFiHandlerClass
{
 private:
	 String _ssid;
	 String _password;
	 String _hostName;
	 WiFiModes _currentWiFiMode;
	 void setupAccessPoint();
 protected:


 public:
	 void init(String ssid, String password);
	 bool isWiFiConnected();
	 WiFiModes getCurrentWiFiMode();
	 String getIpAsString();
	 String getCurrentSsid();
	 String getHostName();
	 String getSsid(int position);
	 int getRssi(int position);
	 int getEncryption(int position);
	 int getAvaiableNetworksCount();
};

extern WiFiHandlerClass WiFiHandler;

#endif

