using SBBotDesktop.Communication;
using SBBotDesktop.Communication.Enums;
using Xunit;

namespace SBBotDesktopTests.IntegrationTests.Communication
{
    public class UdpCommOperationsIntegrationTests
    {
        private const string IpAddress = "172.17.16.9";
        private readonly byte[] _testPacket = { 128, 1, 1, 1 };
        private readonly byte[] _testResponse = { 128, 2, 2, 1, 3 };
        private readonly UdpCommOperations _uco;

        public UdpCommOperationsIntegrationTests()
        {
            _uco = new UdpCommOperations(IpAddress);
        }

        [Fact]
        public void ConnectReturnsTrueIfConnected()
        {
            var result = _uco.Connect();

            Assert.True(result);
        }

        [Fact]
        public void SendCommandReturnsSuccess()
        {
            var result = _uco.SendCommand(UdpRobotCommand.Auto);

            Assert.Equal(result, CommandResult.Success);
        }

    }
}
