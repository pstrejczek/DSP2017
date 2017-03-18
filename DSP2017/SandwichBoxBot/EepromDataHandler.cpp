// 
// 
// 

#include "EepromDataHandler.h"

bool EepromDataHandlerClass::clearEeprom()
{
	for (int i = 0; i < 128; ++i)
	{
		EEPROM.write(i, 0);
	}

	EEPROM.end();

	Serial.println("Eeprom cleared");
	
	return true;
}

String EepromDataHandlerClass::getSsid()
{
	return _ssid;
}

String EepromDataHandlerClass::getPassword()
{
	return _password;
}

bool EepromDataHandlerClass::writeWifiDataToEeeprom(String ssid, String password)
{
	Serial.println(ssid);
	Serial.println(password);

	clearEeprom();
	EEPROM.begin(512);

	for (int i = 0; i < ssid.length(); ++i)
	{
		EEPROM.write(i, ssid[i]);
	}
	for (int i = 0; i < password.length(); ++i)
	{
		EEPROM.write(64 + i, password[i]);
	}

	bool success = EEPROM.commit();

	Serial.println("Saved to eeprom");
	Serial.println(success);

	return success;
}



bool EepromDataHandlerClass::readEepromWiFiParameters()
{
	Serial.println("Starting eeprom read");
	for (int i = 0; i < 64; ++i)
	{
		_ssid += char(EEPROM.read(i));
	}
	for (int i = 64; i < 128; ++i)
	{
		_password += char(EEPROM.read(i));
	}
	Serial.println(_ssid);
	Serial.println(_password);
	Serial.println("Readed from eeprom");

	return true;
}

void EepromDataHandlerClass::init()
{
	EEPROM.begin(512);
}

EepromDataHandlerClass EepromDataHandler;
