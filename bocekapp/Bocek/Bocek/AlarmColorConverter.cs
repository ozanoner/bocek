using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace Bocek
{
    class AlarmColorConverter : Xamarin.Forms.IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool val = (bool)value;
            return val ? Xamarin.Forms.Color.Red : Xamarin.Forms.Color.YellowGreen;

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return false;
        }
    }
}
