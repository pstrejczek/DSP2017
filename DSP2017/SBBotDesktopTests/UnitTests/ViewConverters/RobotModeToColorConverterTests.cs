using SBBotDesktop.Communication;
using SBBotDesktop.ViewConverters;
using System;
using System.Globalization;
using System.Windows.Media;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class RobotModeToColorConverterTests
    {
        [Fact]
        public void ConverterReturnsLimeGreenOnInputAutomatic()
        {
            var converter = new RobotModeToColorConverter();

            Assert.Equal(converter.Convert(RobotMode.Automatic, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.LimeGreen);
        }

        [Fact]
        public void ConverterReturnsYellowOnInputManual()
        {
            var converter = new RobotModeToColorConverter();

            Assert.Equal(converter.Convert(RobotMode.Manual, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.Yellow);
        }
    }
}
