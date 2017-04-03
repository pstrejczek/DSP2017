using System;
using System.Globalization;
using System.Windows.Media;
using SBBotDesktop.ViewConverters;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class ConnectionStateToColorConverterTests
    {
        [Fact]
        public void ConverterReturnsRedOnInputFalse()
        {
            var converter = new ConnectionStateToColorConverter();

            Assert.Equal(converter.Convert(false, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.Red);
        }

        [Fact]
        public void ConverterReturnsLimeOnInputTrue()
        {
            var converter = new ConnectionStateToColorConverter();

            Assert.Equal(converter.Convert(true, typeof(Boolean), null, CultureInfo.CurrentCulture), Brushes.LimeGreen);
        }
    }
}
