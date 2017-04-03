using SBBotDesktop.Communication.Enums;
using SBBotDesktop.ViewConverters;
using System;
using System.Globalization;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class RobotWiFiModeToStringConverterTests
    {
        [Fact]
        public void ConverterReturnsAccessPointOnInputAccessPoint()
        {
            var converter = new RobotWiFiModeToStringConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.AccessPoint, typeof(Boolean), null, CultureInfo.CurrentCulture), "ACCESS POINT");
        }

        [Fact]
        public void ConverterReturnsStationOnInputStation()
        {
            var converter = new RobotWiFiModeToStringConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.Station, typeof(Boolean), null, CultureInfo.CurrentCulture), "STATION");
        }
    }
}
