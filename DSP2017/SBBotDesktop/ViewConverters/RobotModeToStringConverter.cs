using SBBotDesktop.Communication;
using System;
using System.Globalization;
using System.Windows.Data;

namespace SBBotDesktop.ViewConverters
{
    public class RobotModeToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = (RobotMode)value;
            return val == RobotMode.Automatic ? "AUTOMATIC" : "MANUAL";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
