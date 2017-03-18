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
	isInitialized = false;

	delay(1000);

	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work
	EepromWebConfigHandler.readEepromWiFiParameters(); 
	
	_ssid = EepromWebConfigHandler.getSsid();
	_password = EepromWebConfigHandler.getPassword();

	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	WiFi.begin(_ssid.c_str(), _password.c_str());
	
	if(EepromWebConfigHandler._ssid.length() > 1 && isWiFiConnected())
	{
		Serial.println("connected");
		initializeWeb(STATION);
		_currentResponderType = STATION;
	}
	else
	{
		Serial.println("not connected");
		setupAccessPoint();
		_currentResponderType = ACCESS_POINT;
	}
}

WebResponderType WebHandlerClass::getCurrentResponderType()
{
	return _currentResponderType;
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

bool WebHandlerClass::checkMDns()
{
	mdns.update();

	WiFiClient client = server.available();

	delay(10);

	if (!client) {
		return false;
	}

	if (client.connected() && !client.available()) {
		return false;
	}

	Serial.println("");
	Serial.println("New client");

	// Read the first line of HTTP request
	if (client.localPort() != 80)
	{
		client.stopAll();
		return true;
	}
	String request = client.readStringUntil('\r');

	// First line of HTTP request looks like "GET /path HTTP/1.1"
	// Retrieve the "/path" part by finding the spaces
	int addr_start = request.indexOf(' ');
	int addr_end = request.indexOf(' ', addr_start + 1);
	if (addr_start == -1 || addr_end == -1) {
		Serial.print("Invalid request: ");
		Serial.println(request);
		return false;
	}
	request = request.substring(addr_start + 1, addr_end);
	Serial.print("Request: ");
	Serial.println(request);
	client.flush();
	
	if(processGlobalRequests(request, client)) return true; // if global request was processed, do not process requests for AP and STA

	if (getCurrentResponderType() == ACCESS_POINT) {
		processAccessPointWebRequest(request, client);
		
	}
	else // STATION
	{
		processStationWebRequest(request, client);
		
	}

	Serial.println("Request processed");
	return true;
}

void WebHandlerClass::initializeWeb(WebResponderType type)
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
	isInitialized = true;
}

void WebHandlerClass::setupAccessPoint()
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	
	delay(100);
	
	WiFi.softAP("SANDWICHBOXBOT"); // set WiFi Access Point Name
	
	initializeWeb(ACCESS_POINT);

	Serial.println("Acces point initialized ...");
}

void WebHandlerClass::processAccessPointWebRequest(String request, WiFiClient client)
{
	String response;

	if (request == "/")
	{
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>SANDWICHBOXBOT - My IP: ";
		response += getIpAsString();
		response += "<p>";
		response += getAvaiableNetworks();
		response += "<form method='get' action='saveconfig'><label>SSID: </label><input name='ssid' length=32><br><input name='pass' length=64><br><input type='submit' value='SAVE'></form>";
		response += "</html>\r\n\r\n";
		Serial.println("Sending 200");
	}
	else if (request.startsWith("/saveconfig?ssid=")) {
		String qsid;
		qsid = request.substring(request.indexOf("?") + 6, request.indexOf('&'));
		Serial.println(qsid);
		Serial.println("");
		String qpass;
		qpass = request.substring(request.lastIndexOf('=') + 1);
		Serial.println(qpass);
		Serial.println("");

		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
		//response += "Found ";
		//response += request;

		if (EepromWebConfigHandler.writeWifiDataToEeeprom(qsid, qpass))
		{
			response += "<p> configuration SAVED to EEPROM... reboot robot</html>\r\n\r\n";
		}
		else
		{
			response += "<p> configuration save ERROR... try again ! </html>\r\n\r\n";
		}
	}
	else
	{
		response = "HTTP/1.1 404 Not Found\r\n\r\n";
		Serial.println("Sending 404");
	}

	client.print(response);
}

void WebHandlerClass::processStationWebRequest(String request, WiFiClient client)
{
	String response;
	
	if (request == "/")
	{
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from SANDWICHBOXBOT - My IP:";
		response += getIpAsString();
		response += "<p>";
		response += "</html>\r\n\r\n";
		Serial.println("Sending 200 - IP Address info for web browser");
	}
	else if (request.startsWith("/cleareeprom")) {
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from SANDWICHBOXBOT";
		response += "<p>Clearing the EEPROM<p>";
		if (EepromWebConfigHandler.clearEeprom())
		{
			response += "<p>EEPROM cleared ... reboot robot</html>\r\n\r\n";
		}
		else
		{
			response += "<p>EEPROM clear ERROR... try again ! </html>\r\n\r\n";
		}
		Serial.println("Sending 200");
		Serial.println("clearing eeprom");
	}
	else
	{
		response = "HTTP/1.1 404 Not Found\r\n\r\n";
		Serial.println("Sending 404");
	}
	client.print(response);
}

bool WebHandlerClass::processGlobalRequests(String request, WiFiClient client)
{
	bool globalProcessed = false;
	String response;

	if (request.startsWith("/getCurrentMode"))
	{
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
		if (_currentResponderType == ACCESS_POINT) response += "AP";
		else response += "STA";
		response += "<p>";
		response += "</html>\r\n\r\n";
		Serial.println("Sending 200 - current response mode");
		globalProcessed = true;
	}
	else if (request.startsWith("/getIp"))
	{
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>";
		response += getIpAsString();
		response += "<p>";
		response += "</html>\r\n\r\n";
		Serial.println("Sending 200 - IP Address info for app");
		globalProcessed = true;
	}

	client.print(response);
	return(globalProcessed);
}

String WebHandlerClass::getAvaiableNetworks()
{
	int networksCount = WiFi.scanNetworks();

	Serial.println("scanning for networks done ...");

	if (networksCount == 0)
		Serial.println("no networks found");
	else
	{
		Serial.print(networksCount);
		Serial.println(" networks found");
	}

	// prepare HTML formatted network list

	String formatedNetworkList = "<ul>";
	for (int i = 0; i < networksCount; ++i)
	{
		// Print SSID and RSSI for each network found
		formatedNetworkList += "<li>";
		formatedNetworkList += i + 1;
		formatedNetworkList += ": ";
		formatedNetworkList += WiFi.SSID(i);
		formatedNetworkList += " (";
		formatedNetworkList += WiFi.RSSI(i);
		formatedNetworkList += ")";
		formatedNetworkList += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
		formatedNetworkList += "</li>";
	}
	formatedNetworkList += "</ul>";
}

String WebHandlerClass::getIpAsString()
{
	IPAddress ip = WiFi.localIP();
	String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
	return ipStr;
}





