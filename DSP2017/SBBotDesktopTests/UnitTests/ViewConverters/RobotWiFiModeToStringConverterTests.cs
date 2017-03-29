using System;
using System.Globalization;
using SBBotDesktop.Communication;
using SBBotDesktop.ViewConverters;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class RobotWiFiModeToStringConverterTests
    {
        [Fact]
        public void ConverterReturnsNotConnectedOnInputFalse()
        {
            var converter = new RobotWiFiModeToStringConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.AccessPoint, typeof(Boolean), null, CultureInfo.CurrentCulture), "ACCESS POINT");
        }

        [Fact]
        public void ConverterReturnsConnectedOnInputTrue()
        {
            var converter = new RobotWiFiModeToStringConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.Station, typeof(Boolean), null, CultureInfo.CurrentCulture), "STATION");
        }
    }
}
