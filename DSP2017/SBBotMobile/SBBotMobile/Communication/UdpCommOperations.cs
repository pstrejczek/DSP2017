using SBBotMobile.Communication.Enums;
using Sockets.Plugin;

namespace SBBotMobile.Communication
{
    class UdpCommOperations
    {
        private const int Port = 1234;
        private readonly UdpSocketClient _client;
        private readonly string _ipAddress;
        private bool _isConnected = false;

        public UdpCommOperations(string ipAddress)
        {
            _ipAddress = ipAddress;
            _client = new UdpSocketClient();
            Connect();
        }

        public async void Connect()
        {
            await _client.ConnectAsync(_ipAddress, Port);
            _isConnected = true;
        }

        public bool IsConnected()
        {
            return _isConnected;
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
            if(_isConnected) Connect();

            var command = PrepareCommand(robotCommand);

            _client.SendAsync(command, command.Length);

            //var response = _client.Receive(ref _remoteEndPoint);

            //if (!IsPacketCorrect(response)) return CommandResult.Error;
            //if (!IsCommandConfirmed(response, robotCommand)) return CommandResult.Error;

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

    }
}
