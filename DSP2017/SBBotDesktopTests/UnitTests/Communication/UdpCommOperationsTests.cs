using SBBotDesktop.Communication;
using SBBotDesktop.Communication.Enums;
using Xunit;

namespace SBBotDesktopTests.UnitTests.Communication
{
    public class UdpCommOperationsTests
    {
        private const string IpAddress = "172.17.16.9";
        private readonly byte[] _testPacket = { 128, 1, 1, 1 };
        private readonly byte[] _testResponse = { 128, 2, 2, 1, 3 };
        private readonly UdpCommOperations _uco;

        public UdpCommOperationsTests()
        {
            _uco = new UdpCommOperations(IpAddress);
        }


        [Fact]
        public void CalculateChecksumReturnsChecksumValue()
        {
            var checkSum = _uco.CalculateChecksum(_testPacket, _testPacket[1]);

            Assert.Equal(checkSum, _testPacket[3]);
        }

        [Fact]
        public void PrepareCommandReturnsArrayOfBytes()
        {
            var command = _uco.PrepareCommand(UdpRobotCommand.Auto);

            Assert.NotEmpty(command);
            Assert.Equal(command.Length, 4);
        }

        [Fact]
        public void IsPacketCorrectReturnsTrueForTestPacket()
        {
            var result = _uco.IsPacketCorrect(_testPacket);

            Assert.True(result);
        }

        [Fact]
        public void IsCommandConfirmedReturnsTrueForTestResponse()
        {
            var result = _uco.IsCommandConfirmed(_testResponse, UdpRobotCommand.Auto);

            Assert.True(result);
        }
    }
}
