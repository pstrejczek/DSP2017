using SBBotDesktop.Communication;
using SBBotDesktop.Communication.Enums;
using Xunit;

namespace SBBotDesktopTests.IntegrationTests.Communication
{
    public class WebOperationsIntegrationTests
    {
        private readonly WebOperations _webOperations;

        public WebOperationsIntegrationTests()
        {
            _webOperations = new WebOperations("172.17.16.9");
        }

        [Fact]
        public void GetCurrentWiFiModeReturnsCurrentWiFiMode()
        {
            var wifiMode = _webOperations.GetCurrentWiFiMode();

            Assert.NotEqual(wifiMode, RobotWiFiMode.Error);
        }

        [Fact]
        public void GetCurrentRobotModeReturnsCurrentRobotMode()
        {
            var robotMode = _webOperations.GetCurrentRobotMode();

            Assert.NotEqual(robotMode, RobotMode.Error);
        }

        [Fact]
        public void GetAvaiableNetworksListReturnsNetworksListAsListOfString()
        {
            var networks = _webOperations.GetAvaiableNetworks();

            Assert.NotEmpty(networks);
        }
    }
}
