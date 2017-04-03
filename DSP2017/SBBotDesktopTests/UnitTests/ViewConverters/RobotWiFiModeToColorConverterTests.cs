using SBBotDesktop.Communication.Enums;
using SBBotDesktop.ViewConverters;
using System;
using System.Globalization;
using System.Windows.Media;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class RobotWiFiModeToColorConverterTests
    {
        [Fact]
        public void ConverterReturnsNotConnectedOnInputFalse()
        {
            var converter = new RobotWifiModeToColorConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.AccessPoint, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.Yellow);
        }

        [Fact]
        public void ConverterReturnsConnectedOnInputTrue()
        {
            var converter = new RobotWifiModeToColorConverter();

            Assert.Equal(converter.Convert(RobotWiFiMode.Station, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.LimeGreen);
        }
    }
}
