// 
// 
// 

#include "WiFiHandler.h"

void WiFiHandlerClass::init(String ssid, String password)
{
	_hostName = "sandwichboxbot";
	_ssid = ssid;
	_password = password;
	
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	WiFi.hostname(_hostName);

	WiFi.begin(_ssid.c_str(), _password.c_str());

	if (_ssid.length() > 1 && isWiFiConnected())
	{
		Serial.println("connected");
		_currentWiFiMode = WFM_STATION;
	}
	else
	{
		Serial.println("not connected");
		setupAccessPoint();
		_currentWiFiMode = WFM_ACCESS_POINT;
	}
}

void WiFiHandlerClass::setupAccessPoint()
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();

	delay(100);

	WiFi.softAP("SANDWICHBOXBOT"); // set WiFi Access Point Name

	Serial.println("Acces point initialized ...");
}

bool WiFiHandlerClass::isWiFiConnected()
{
	int noOfTries = 0;

	while (noOfTries < 3) {
		int cResult = WiFi.waitForConnectResult();
		if (cResult == WL_CONNECTED) { return(true); }
		Serial.println(cResult);
		WiFi.reconnect();
		noOfTries++;
	}

	return(false);
}

WiFiModes WiFiHandlerClass::getCurrentWiFiMode()
{
	return _currentWiFiMode;
}

String WiFiHandlerClass::getIpAsString()
{
	IPAddress ip = WiFi.localIP();
	String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
	return ipStr;
}

String WiFiHandlerClass::getCurrentSsid()
{
	return WiFi.SSID();
}

String WiFiHandlerClass::getHostName()
{
	return _hostName;
}

String WiFiHandlerClass::getSsid(int position)
{
	return WiFi.SSID(position);
}

int WiFiHandlerClass::getRssi(int position)
{
	return WiFi.RSSI(position);
}

int WiFiHandlerClass::getEncryption(int position)
{
	return WiFi.encryptionType(position);
}

int WiFiHandlerClass::getAvaiableNetworksCount()
{
	return WiFi.scanNetworks();
}






