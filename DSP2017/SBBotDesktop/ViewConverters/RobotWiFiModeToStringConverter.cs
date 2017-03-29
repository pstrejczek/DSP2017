using SBBotDesktop.Communication;
using System;
using System.Globalization;
using System.Windows.Data;

namespace SBBotDesktop.ViewConverters
{
    public class RobotWiFiModeToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = (RobotWiFiMode)value;

            return val == RobotWiFiMode.AccessPoint ? "ACCESS POINT" : "STATION";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
