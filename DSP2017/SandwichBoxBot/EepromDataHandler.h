// EepromDataHandler.h

#ifndef _EEPROMDATAHANDLER_h
#define _EEPROMDATAHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <EEPROM.h>

class EepromDataHandlerClass
{
 protected:


 public:
	 String _ssid;
	 String _password;
	 String getSsid();
	 String getPassword();
	 bool clearEeprom();
	 bool writeWifiDataToEeeprom(String ssid, String password);
	 bool readEepromWiFiParameters();
	 void init();
};

extern EepromDataHandlerClass EepromDataHandler;

#endif

