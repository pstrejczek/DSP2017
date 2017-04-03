using System;
using System.Globalization;
using SBBotDesktop.ViewConverters;
using Xunit;

namespace SBBotDesktopTests.UnitTests.ViewConverters
{
    public class ConnectionStateToStringConverterTests
    {
        [Fact]
        public void ConverterReturnsNotConnectedOnInputFalse()
        {
            var converter = new ConnectionStateToStringConverter();

            Assert.Equal(converter.Convert(false, typeof(Boolean), null, CultureInfo.CurrentCulture), "NOT CONNECTED");
        }

        [Fact]
        public void ConverterReturnsConnectedOnInputTrue()
        {
            var converter = new ConnectionStateToStringConverter();

            Assert.Equal(converter.Convert(true, typeof(Boolean), null, CultureInfo.CurrentCulture), "CONNECTED");
        }
    }
}
