using SBBotDesktop.Communication;
using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace SBBotDesktop.ViewConverters
{
    public class RobotModeToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = (RobotMode)value;
            return val == RobotMode.Automatic ? Brushes.LimeGreen : Brushes.Yellow;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
