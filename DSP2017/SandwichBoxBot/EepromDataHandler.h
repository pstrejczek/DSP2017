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
 private:
	String _ssid;
	String _password;

 protected:
	
 public:
	 
	 String getSsid();
	 String getPassword();
	 bool clearEeprom();
	 bool writeWifiDataToEeeprom(String ssid, String password);
	 bool readEepromWiFiParameters();
	 void init();
};

extern EepromDataHandlerClass EepromDataHandler;

#endif

