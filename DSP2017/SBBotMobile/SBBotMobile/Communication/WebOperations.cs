using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using SBBotMobile.Communication.Enums;

namespace SBBotMobile.Communication
{
    class WebOperations
    {
        private readonly string _robotIp;
        private readonly HttpClient _httpClient;

        public WebOperations(string robotIp)
        {
            _robotIp = robotIp;
            _httpClient = new HttpClient();
        }

        public async Task<RobotWiFiMode> GetCurrentWiFiMode()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("getCurrentMode"));

            if (response.Contains("STA")) return RobotWiFiMode.Station;
            else if (response.Contains("AP")) return RobotWiFiMode.AccessPoint;
            else return RobotWiFiMode.Error;
        }

        public async Task<RobotMode> GetCurrentRobotMode()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("getCurrentRobotMode"));

            if (response.Contains("AUTOMATIC")) return RobotMode.Automatic;
            else if (response.Contains("MANUAL")) return RobotMode.Manual;
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

        public async Task<List<string>> GetAvaiableNetworks()
        {
            var response = new List<string>();

            var stream = await _httpClient.GetStreamAsync(PrepareWebAddress("getAvaiableNetworks"));

            if (stream != null)
            {
                using (var reader = new StreamReader(stream))
                {
                    while (!reader.EndOfStream) response.Add(reader.ReadLine());
                }
            }

            return response;
        }

        public async Task<CommandResult> ClearEeprom()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("cleareeprom"));

            return response.Contains("ERROR") ? CommandResult.Error : CommandResult.Success;
        }

        public async Task<CommandResult> SetNetworkCredentials(string networkSsid, string password)
        {
            var values = new List<KeyValuePair<string, string>>
            {
                new KeyValuePair<string, string>("ssid", networkSsid),
                new KeyValuePair<string, string>("pass", password)
            };

            var content = new FormUrlEncodedContent(values);

            var response = await _httpClient.PostAsync(PrepareWebAddress("saveconfig"), content);

            var rep = await response.Content.ReadAsStringAsync();

            return rep.Contains("ERROR") ? CommandResult.Error : CommandResult.Success;
        }
    }
}
