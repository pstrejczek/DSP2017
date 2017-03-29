using System;
using System.Globalization;
using System.Windows.Data;

namespace SBBotDesktop.ViewModels.Converters
{
    public class ConnectionStateToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = value != null && (bool)value;

            return val ? "CONNECTED" : "NOT CONNECTED";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
