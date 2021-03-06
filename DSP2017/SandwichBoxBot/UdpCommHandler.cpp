// 
// 
// 

#include "UdpCommHandler.h"

WiFiUDP Udp;
byte incomingPacket[UDP_TX_PACKET_MAX_SIZE];


void UdpCommHandlerClass::init(int localPort)
{
	_localUdpPort = localPort;
	Udp.begin(_localUdpPort);
	isInitialized = true;
}

byte UdpCommHandlerClass::calculateChecksum(unsigned char* packet, int dataLength)
{
	int sum = 0;

	for(int i=0; i < dataLength; i++)
	{
		sum += packet[2 + i];
	}

	return sum;
}

bool UdpCommHandlerClass::isFrameCorrect(unsigned char* packet, int packetLength)
{
	int error_count = 0;

	if (packet[0] != 128) error_count++; //check header
	byte dataLength = packet[1]; // get data length

	byte checkSum = packet[1 + dataLength + 1]; // get checksum
	byte calculatedCheckSum = calculateChecksum(packet, dataLength); // calculate checksum

	if (checkSum != calculatedCheckSum) error_count++;

	if (error_count == 0) return true;
	else return false;
}

void UdpCommHandlerClass::sendUdpPacket(unsigned char* packet, int packetLength)
{
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	Udp.write(packet, packetLength);
	Udp.endPacket();
}

void UdpCommHandlerClass::sendCommandError()
{
	byte errorResponse[4];

	errorResponse[0] = 128; //header
	errorResponse[1] = 1; //data length
	errorResponse[2] = CF_ERROR; //error code
	
	byte checkSum = calculateChecksum(errorResponse, 1);
	errorResponse[3] = checkSum;

	sendUdpPacket(errorResponse, 4);
}

void UdpCommHandlerClass::confirmCommand(int command)
{
	byte confirmResponse[5];

	confirmResponse[0] = 128; //header
	confirmResponse[1] = 2; //data length
	confirmResponse[2] = CF_CONFIRM;
	confirmResponse[3] = command;

	byte checkSum = calculateChecksum(confirmResponse, 2);
	confirmResponse[4] = checkSum;

	sendUdpPacket(confirmResponse, 5);
}

Command UdpCommHandlerClass::processCommandRequest()
{
	int packetSize = Udp.parsePacket();
	
	if (packetSize == 0)
	{
		return C_NONE;
	}

	int commandLength = Udp.read(incomingPacket, UDP_TX_PACKET_MAX_SIZE);
	//Udp.flush();
	
	if (!isFrameCorrect(incomingPacket, commandLength))
	{
		sendCommandError();
		return C_PACKET_ERROR;
	}

	int commandFunction = incomingPacket[2];
	
	if (commandFunction != CF_SET) // 
	{
		sendCommandError();
		return C_PACKET_ERROR;
	}
	
	int command = incomingPacket[3];

	confirmCommand(command);
	
	switch(command)
	{
		case 1: return C_AUTO;
		case 2: return C_MANUAL;
		case 3: return C_FORWARD;
		case 4: return C_BACKWARD;
		case 5: return C_LEFT;
		case 6: return C_RIGHT;
		case 7: return C_STOP;
		default: return C_PACKET_ERROR;
	}
}






