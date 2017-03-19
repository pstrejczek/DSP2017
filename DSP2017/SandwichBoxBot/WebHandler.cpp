// 
// 
// 

#include "WebHandler.h"

EepromDataHandlerClass EepromWebConfigHandler;
ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer updateServer;

void WebHandlerClass::init()
{
	isInitialized = false;

	delay(100);

	EepromWebConfigHandler.init(); // you cannot start eeprom.begin in constructor it does not work
	EepromWebConfigHandler.readEepromWiFiParameters(); 
	
	_ssid = EepromWebConfigHandler.getSsid();
	_password = EepromWebConfigHandler.getPassword();
	
	_hostName = "sandwichboxbot";

	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	WiFi.hostname(_hostName);

	WiFi.begin(_ssid.c_str(), _password.c_str());
	
	if(_ssid.length() > 1 && isWiFiConnected())
	{
		Serial.println("connected");
		initializeWeb(STATION);
		_currentResponderType = STATION;
	}
	else
	{
		Serial.println("not connected");
		setupAccessPoint();
		initializeWeb(ACCESS_POINT);
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

bool WebHandlerClass::processRequest()
{
	MDNS.update();
	
	webServer.handleClient();
		
	return true;
}

void webHandleNotFound()
{
	String response = "HTTP/1.1 404 Not Found\r\n\r\n";
	webServer.send(404, "text/html", response);
}

void WebHandlerClass::initializeWeb(WebResponderType type)
{
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println(WiFi.localIP());
	Serial.println(WiFi.softAPIP());

	// Start MDNS
	if (!MDNS.begin(_hostName.c_str())) {
		Serial.println("Error setting up MDNS responder!");
		while (true) {
			delay(1000);
		}
	}
	
	Serial.println("mDNS started");
	
	// prepare routing
	webServer.on("/", std::bind(&WebHandlerClass::webHandleRoot, this));
	webServer.on("/cleareeprom", std::bind(&WebHandlerClass::webHandleClearEeprom, this));
	webServer.on("/getCurrentMode", std::bind(&WebHandlerClass::webHandleCurrentMode, this));
	webServer.on("/getIp", std::bind(&WebHandlerClass::webHandleGetIp, this));
	webServer.on("/getCurrentNetwork", std::bind(&WebHandlerClass::webHandleGetIp, this));
	webServer.on("/saveconfig", HTTP_POST, std::bind(&WebHandlerClass::webHandleChangeCredentials, this));
	webServer.onNotFound(std::bind(&WebHandlerClass::webHandleNotFound, this));
	// Setup the update server
	updateServer.setup(&webServer);
	
	// Start web server
	webServer.begin();
	
	// Register service to be discovered in bonjour
	MDNS.addService("sbbot", "tcp", 1234);

	isInitialized = true;
	Serial.println("intialization end");
}

void WebHandlerClass::setupAccessPoint()
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	
	delay(100);
	
	WiFi.softAP("SANDWICHBOXBOT"); // set WiFi Access Point Name
	
	Serial.println("Acces point initialized ...");
}

void WebHandlerClass::webHandleRoot()
{
	String response;

	switch (_currentResponderType)
	{
	case ACCESS_POINT:
		response = "<html>SANDWICHBOXBOT - My IP: ";
		response += getIpAsString();
		response += "<p>Avaiable Networks:</p><p>";
		response += getAvaiableNetworks();
		response += "<form method='post' action='saveconfig'><label>SSID: </label><input name='ssid' length=32><br><label>PASS: </label><input type='password' name='pass' length=64><br><input type='submit' value='SAVE'></form>";
		response += "</html>";
		Serial.println("Sending 200 - ssid and password set page");
		break;
	case STATION:
		response = "<html>Hello from SANDWICHBOXBOT <p>Network: ";
		response += WiFi.SSID();
		response += "</p><p>My IP: ";
		response += getIpAsString();
		response += "</p><p>";
		response += "</html>";
		Serial.println("Sending 200 - IP Address info for web browser");
		break;
	default:
		break;
	}
	webServer.send(200, "text/html", response);
}

void WebHandlerClass::webHandleChangeCredentials()
{
	if (!webServer.hasArg("ssid") || !webServer.hasArg("pass")) { webServer.send(500, "text/plain", "BAD ARGS"); return; }
	String ssid = webServer.arg("ssid");
	String password = webServer.arg("pass");

	Serial.println(ssid);
	Serial.println(password);

	String response = "<!DOCTYPE HTML><html>";

	if (EepromWebConfigHandler.writeWifiDataToEeeprom(ssid, password))
	{
		response += "<p> configuration SAVED to EEPROM... please cycle power on the robot</html>";
	}
	else
	{
		response += "<p> configuration save ERROR... try again ! </html>";
	}

	webServer.send(200, "text/html", response);
}

void WebHandlerClass::webHandleClearEeprom()
{
	String response;

	response = "<!DOCTYPE HTML><html>Hello from SANDWICHBOXBOT";
	response += "<p>Clearing the EEPROM<p>";
	if (EepromWebConfigHandler.clearEeprom())
	{
		response += "<p>EEPROM cleared ... please cycle power on the robot</html>";
	}
	else
	{
		response += "<p>EEPROM clear ERROR... try again ! </html>";
	}
	Serial.println("Sending 200");
	Serial.println("clearing eeprom");
	webServer.send(200, "text/html", response);
}

void WebHandlerClass::webHandleGetIp()
{
	String response = getIpAsString();
	Serial.println("Sending 200 - IP Address info for app");
	webServer.send(200, "text/plain", response);
}

void WebHandlerClass::webHandleCurrentMode()
{
	String response;

	if (_currentResponderType == ACCESS_POINT) response = "AP";
	else response = "STA";
	Serial.println("Sending 200 - current response mode");
	webServer.send(200, "text/plain", response);
}

void WebHandlerClass::webHandleGetCurrentNetwork()
{
	String response = WiFi.SSID();
	Serial.println("Sending 200 - current network");
	webServer.send(200, "text/plain", response);
}

void WebHandlerClass::webHandleNotFound()
{
	Serial.println("Sending 404");
	webServer.send(404, "text/html", "");
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

	return formatedNetworkList;
}

String WebHandlerClass::getIpAsString()
{
	IPAddress ip = WiFi.localIP();
	String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
	return ipStr;
}





