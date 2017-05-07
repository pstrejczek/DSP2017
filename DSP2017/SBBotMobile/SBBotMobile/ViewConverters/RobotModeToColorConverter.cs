using System;
using System.Globalization;
using SBBotMobile.Communication.Enums;
using Xamarin.Forms;

namespace SBBotMobile.ViewConverters
{
    public class RobotModeToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = (RobotMode)value;
            return val == RobotMode.Automatic ? Color.LimeGreen : Color.Goldenrod;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
