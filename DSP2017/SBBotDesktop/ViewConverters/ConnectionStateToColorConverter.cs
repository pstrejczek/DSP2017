using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace SBBotDesktop.ViewModels.Converters
{
    public class ConnectionStateToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var val = value != null && (bool)value;

            return val ? Brushes.LimeGreen : Brushes.Red;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
