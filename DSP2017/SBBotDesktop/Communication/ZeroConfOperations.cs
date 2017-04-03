using System.Linq;
using System.Threading.Tasks;
using Zeroconf;

namespace SBBotDesktop.Communication
{
    public class ZeroConfOperations
    {
        public async Task<string> GetRobotIp()
        {
            var robotIp = string.Empty;

            var results = await ZeroconfResolver.ResolveAsync("_sbbot._tcp.local.");

            if (!(results?.Count > 0)) return robotIp;

            var robot = results.FirstOrDefault(r => r.DisplayName == "sandwichboxbot");
            if (robot != null) robotIp = robot.IPAddress;

            return robotIp;
        }
    }
}
