using SBBotDesktop.Communication;
using System;
using Xunit;

namespace SBBotDesktopTests.UnitTests.Communication
{
    public class WebOperationsTests
    {
        private readonly WebOperations _webOperations;

        public WebOperationsTests()
        {
            _webOperations = new WebOperations("172.17.16.9");
        }

        [Fact]
        public void PrepareWebAddressReturnsValidHttpAddress()
        {
            var address = _webOperations.PrepareWebAddress("/getCurrentMode");

            Assert.Equal(Uri.IsWellFormedUriString(address, UriKind.Absolute), true);
        }
    }
}
