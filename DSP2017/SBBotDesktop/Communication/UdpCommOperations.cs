using System;
using System.Net;
using System.Net.Sockets;
using SBBotDesktop.Communication.Enums;

namespace SBBotDesktop.Communication
{
    public class UdpCommOperations : IDisposable
    {
        private const int Port = 1234;
        private readonly UdpClient _client;
        private IPEndPoint _remoteEndPoint;

        public UdpCommOperations(string ipAddress)
        {
            _remoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), Port);
            _client = new UdpClient();
            Connect();
        }

        public bool Connect()
        {
            if (!IsConnected()) _client.Connect(_remoteEndPoint);

            return IsConnected();
        }

        public bool IsConnected()
        {
            return _client.Client.Connected;
        }

        public byte CalculateChecksum(byte[] packet, int dataLength)
        {
            var sum = 0;

            for (var i = 0; i < dataLength; i++)
            {
                sum += packet[2 + i];
            }

            return (byte)sum;
        }

        public byte[] PrepareCommand(UdpRobotCommand robotCommand)
        {
            var command = new byte[5];

            command[0] = 128;
            command[1] = 2; //data length
            command[2] = 1; //set command
            command[3] = (byte)robotCommand;
            command[4] = CalculateChecksum(command, 2);

            return command;
        }

        public CommandResult SendCommand(UdpRobotCommand robotCommand)
        {
            if (!IsConnected()) Connect();

            var command = PrepareCommand(robotCommand);

            _client.Send(command, command.Length);

            var response = _client.Receive(ref _remoteEndPoint);

            if (!IsPacketCorrect(response)) return CommandResult.Error;
            if (!IsCommandConfirmed(response, robotCommand)) return CommandResult.Error;

            return CommandResult.Success;
        }

        public bool IsPacketCorrect(byte[] packet)
        {
            var errorCount = 0;

            if (packet.Length < 4) return false;
            if (packet[0] != 128) errorCount++;

            var dataLength = packet[1];
            var checkSum = packet[dataLength + 2];
            var calculatedChecksum = CalculateChecksum(packet, dataLength);

            if (checkSum != calculatedChecksum) errorCount++;

            return errorCount <= 0;
        }

        public bool IsCommandConfirmed(byte[] packet, UdpRobotCommand command)
        {
            return packet[2] == 2 && packet[3] == (byte)command;
        }

        public void Dispose()
        {
            if (_client.Client.Connected) _client.Close();
            ((IDisposable)_client)?.Dispose();
        }
    }
}
