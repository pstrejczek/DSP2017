// UdpCommHandler.h

#ifndef _UDPCOMMHANDLER_h
#define _UDPCOMMHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <WiFiUdp.h>

typedef enum CommandFunction
{
	CF_SET = 1,
	CF_CONFIRM = 2,
	CF_ERROR = 3
};

typedef enum Command
{
	C_NONE,
	C_PACKET_ERROR,
	C_AUTO,
	C_MANUAL,
	C_FORWARD,
	C_BACKWARD,
	C_LEFT,
	C_RIGHT
};

class UdpCommHandlerClass
{
 private:
	 int _localUdpPort;
	 byte calculateChecksum(unsigned char* packet, int dataLength);
	 bool isFrameCorrect(unsigned char* packet, int packetLength);
	 void sendUdpPacket(unsigned char *packet, int packetLength);
	 void sendCommandError();
	 void confirmCommand(int command);
 protected:


 public:
	void init(int localPort);
	Command processCommandRequest();

	bool isInitialized;
};

extern UdpCommHandlerClass UdpCommHandler;

#endif

