using SBBotDesktop.Communication;
using System.Net;
using Xunit;

namespace SBBotDesktopTests.IntegrationTests.Communication
{
    public class ZeroConfOperationsIntegrationTests
    {
        [Fact]
        public async void GetRobotIpAddressReturnsRobotIp()
        {
            IPAddress ipAddress;

            var zco = new ZeroConfOperations();
            var ipString = await zco.GetRobotIp();

            Assert.NotEqual(ipString, string.Empty);
            Assert.Equal(IPAddress.TryParse(ipString, out ipAddress), true);
        }
    }
}
