// 
// 
// 

#include "WebHandler.h"

EepromDataHandlerClass EepromWebConfigHandler;
MDNSResponder mdns;
WiFiServer server(80);
String st;

void WebHandlerClass::init()
{
	delay(1000);

	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work

	EepromWebConfigHandler.readEepromWiFiParameters();
	
	_ssid = EepromWebConfigHandler.getSsid();
	_password = EepromWebConfigHandler.getPassword();

	WiFi.mode(WIFI_STA);
	WiFi.begin(_ssid.c_str(), _password.c_str());
	
	if(EepromWebConfigHandler._ssid.length() > 1 && isWiFiConnected())
	{
		Serial.println("connected");
		initializeWeb(0);
	}
	else
	{
		Serial.println("not connected");
		setupAp();
	}
}

bool WebHandlerClass::isWiFiConnected()
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

int WebHandlerClass::checkMDns(int type)
{
	mdns.update();

	WiFiClient client = server.available();
	if (!client) {
		return(20);
	}

	while (client.connected() && !client.available()) {
		delay(1);
	}
	Serial.println("");
	Serial.println("New client");

	// Read the first line of HTTP request
	String req = client.readStringUntil('\r');

	// First line of HTTP request looks like "GET /path HTTP/1.1"
	// Retrieve the "/path" part by finding the spaces
	int addr_start = req.indexOf(' ');
	int addr_end = req.indexOf(' ', addr_start + 1);
	if (addr_start == -1 || addr_end == -1) {
		Serial.print("Invalid request: ");
		Serial.println(req);
		return(20);
	}
	req = req.substring(addr_start + 1, addr_end);
	Serial.print("Request: ");
	Serial.println(req);
	client.flush();
	
	String s;
	
	if (type == 1) {
		if (req == "/")
		{
			IPAddress ip = WiFi.softAPIP();
			String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
			s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>SANDWICHBOXBOT - My IP: ";
			s += ipStr;
			s += "<p>";
			s += st;
			s += "<form method='get' action='saveconfig'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
			s += "</html>\r\n\r\n";
			Serial.println("Sending 200");
		}
		else if (req.startsWith("/saveconfig?ssid=")) {
			// /a?ssid=blahhhh&pass=poooo
			String qsid;
			qsid = req.substring(req.indexOf("?") + 6, req.indexOf('&'));
			Serial.println(qsid);
			Serial.println("");
			String qpass;
			qpass = req.substring(req.lastIndexOf('=') + 1);
			Serial.println(qpass);
			Serial.println("");

			s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from SANDWICHBOXBOT ";
			s += "Found ";
			s += req;

			if (EepromWebConfigHandler.writeWifiDataToEeeprom(qsid, qpass))
			{
				s += "<p> configuration saved to EEPROM... reboot robot</html>\r\n\r\n";
			}
			else
			{
				s += "<p> configuration save ERROR... try again ! </html>\r\n\r\n";
			}
	    }
		else
		{
			s = "HTTP/1.1 404 Not Found\r\n\r\n";
			Serial.println("Sending 404");
		}
	}
	else
	{
		if (req == "/")
		{
			IPAddress ip = WiFi.localIP();
			String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
			s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from SANDWICHBOXBOT - My IP:";
			s += ipStr;
			s += "<p>";
			s += "</html>\r\n\r\n";
			Serial.println("Sending 200");
		}
		else if (req.startsWith("/cleareeprom")) {
			s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from SANDWICHBOXBOT";
			s += "<p>Clearing the EEPROM<p>";
			s += "</html>\r\n\r\n";
			Serial.println("Sending 200");
			Serial.println("clearing eeprom");
			//for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
			//EEPROM.commit();
		}
		else
		{
			s = "HTTP/1.1 404 Not Found\r\n\r\n";
			Serial.println("Sending 404");
		}
	}
	client.print(s);
	return(20);
}

void WebHandlerClass::initializeWeb(int type)
{
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println(WiFi.localIP());
	Serial.println(WiFi.softAPIP());
	if (!mdns.begin("sandwichboxbot", WiFi.localIP())) {
		Serial.println("Error setting up MDNS responder!");
		while (true) {
			delay(1000);
		}
	}
	Serial.println("mDNS responder started");
	server.begin();
	Serial.println("Server started");
	int b = 20;
	int c = 0;
	while (b == 20) {
		b = checkMDns(type);
	}
}

void WebHandlerClass::setupAp()
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
		Serial.println("no networks found");
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			delay(10);
		}
	}
	Serial.println("");
	st = "<ul>";
	for (int i = 0; i < n; ++i)
	{
		// Print SSID and RSSI for each network found
		st += "<li>";
		st += i + 1;
		st += ": ";
		st += WiFi.SSID(i);
		st += " (";
		st += WiFi.RSSI(i);
		st += ")";
		st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
		st += "</li>";
	}
	st += "</ul>";
	delay(100);
	WiFi.softAP("SANDWICHBOXBOT");
	Serial.println("softap");
	Serial.println("");
	initializeWeb(1);
	Serial.println("over");
}



