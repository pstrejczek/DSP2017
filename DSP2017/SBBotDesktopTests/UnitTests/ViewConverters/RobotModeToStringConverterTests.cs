using SBBotDesktop.Communication;
using SBBotDesktop.ViewConverters;
using System;
using System.Globalization;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class RobotModeToStringConverterTests
    {
        [Fact]
        public void ConverterReturnsAutomaticOnInputAutomatic()
        {
            var converter = new RobotModeToStringConverter();

            Assert.Equal(converter.Convert(RobotMode.Automatic, typeof(Boolean), null, CultureInfo.CurrentCulture), "AUTOMATIC");
        }

        [Fact]
        public void ConverterReturnsManualOnInputManual()
        {
            var converter = new RobotModeToStringConverter();

            Assert.Equal(converter.Convert(RobotMode.Manual, typeof(Boolean), null, CultureInfo.CurrentCulture), "MANUAL");
        }
    }
}
