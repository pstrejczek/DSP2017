using SBBotDesktop.Communication;
using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace SBBotDesktop.ViewConverters
{
    public class RobotWifiModeToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = (RobotWiFiMode)value;

            return val == RobotWiFiMode.AccessPoint ? Brushes.Yellow : Brushes.LimeGreen;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
