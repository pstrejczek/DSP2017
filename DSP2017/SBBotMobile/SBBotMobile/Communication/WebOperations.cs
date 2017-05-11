using System.Collections.Generic;
using System.IO;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using SBBotMobile.Communication.Enums;

namespace SBBotMobile.Communication
{
    public static class WebOperations
    {
        private static string _robotIp;
        private static HttpClient _httpClient;

        public static void Initialize(string robotIp)
        {
            _robotIp = robotIp;
            _httpClient = new HttpClient();
        }

        public static async Task<RobotWiFiMode> GetCurrentWiFiMode()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("getCurrentMode"));

            if (response.Contains("STA")) return RobotWiFiMode.Station;
            else if (response.Contains("AP")) return RobotWiFiMode.AccessPoint;
            else return RobotWiFiMode.Error;
        }

        public static async Task<RobotMode> GetCurrentRobotMode()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("getCurrentRobotMode"));

            if (response.Contains("AUTOMATIC")) return RobotMode.Automatic;
            else if (response.Contains("MANUAL")) return RobotMode.Manual;
            else return RobotMode.Error;
        }

        public static string PrepareWebAddress(string parameter)
        {
            var webAddress = new StringBuilder();
            webAddress.Append("http://");
            webAddress.Append(_robotIp);
            webAddress.Append("/");
            webAddress.Append(parameter);

            return webAddress.ToString();
        }

        public static async Task<List<string>> GetAvaiableNetworks()
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

        public static async Task<CommandResult> ClearEeprom()
        {
            var response = await _httpClient.GetStringAsync(PrepareWebAddress("cleareeprom"));

            return response.Contains("ERROR") ? CommandResult.Error : CommandResult.Success;
        }

        public static async Task<CommandResult> SetNetworkCredentials(string networkSsid, string password)
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
