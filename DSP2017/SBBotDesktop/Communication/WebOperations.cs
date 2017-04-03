using SBBotDesktop.Communication.Enums;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Net;
using System.Text;

namespace SBBotDesktop.Communication
{
    public class WebOperations
    {
        private readonly string _robotIp;
        private readonly WebClient _wc;

        public WebOperations(string robotIp)
        {
            _robotIp = robotIp;
            _wc = new WebClient();
        }

        public RobotWiFiMode GetCurrentWiFiMode()
        {
            var response = string.Empty;

            var stream = _wc.OpenRead(PrepareWebAddress("getCurrentMode"));

            if (stream != null)
            {
                using (var reader = new StreamReader(stream))
                {
                    response = reader.ReadToEnd();
                }
            }

            if (response.Contains("STA")) return RobotWiFiMode.Station;
            else if (response.Contains("AP")) return RobotWiFiMode.AccessPoint;
            else return RobotWiFiMode.Error;
        }

        public RobotMode GetCurrentRobotMode()
        {
            var response = string.Empty;

            var stream = _wc.OpenRead(PrepareWebAddress("getCurrentRobotMode"));

            if (stream != null)
            {
                using (var reader = new StreamReader(stream))
                {
                    response = reader.ReadToEnd();
                }
            }

            if (response.Contains("AUTOMATIC")) return RobotMode.Automatic;
            else if (response.Contains("MANUAL")) return RobotMode.Automatic;
            else return RobotMode.Error;
        }

        public string PrepareWebAddress(string parameter)
        {
            var webAddress = new StringBuilder();
            webAddress.Append("http://");
            webAddress.Append(_robotIp);
            webAddress.Append("/");
            webAddress.Append(parameter);

            return webAddress.ToString();
        }

        public List<string> GetAvaiableNetworks()
        {
            var response = new List<string>();

            var stream = _wc.OpenRead(PrepareWebAddress("getAvaiableNetworks"));

            if (stream != null)
            {
                using (var reader = new StreamReader(stream))
                {
                    while (!reader.EndOfStream) response.Add(reader.ReadLine());
                }
            }

            return response;
        }

        public CommandResult ClearEeprom()
        {
            var response = string.Empty;

            var stream = _wc.OpenRead(PrepareWebAddress("cleareeprom"));

            if (stream != null)
            {
                using (var reader = new StreamReader(stream))
                {
                    response = reader.ReadToEnd();
                }
            }
            return response.Contains("ERROR") ? CommandResult.Error : CommandResult.Success;
        }

        public CommandResult SetNetworkCredentials(string networkSsid, string password)
        {
            var values = new NameValueCollection
            {
                ["ssid"] = networkSsid,
                ["pass"] = password
            };

            var byteResponse = _wc.UploadValues(PrepareWebAddress("saveconfig"), values);

            var response = Encoding.Default.GetString(byteResponse);

            return response.Contains("ERROR") ? CommandResult.Error : CommandResult.Success;
        }
    }
}
