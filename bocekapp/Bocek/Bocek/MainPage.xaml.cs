using Xamarin.Forms;

namespace Bocek
{
    public partial class MainPage : ContentPage
    {

        public MainPage()
        {
            
            InitializeComponent();
            Label caption = new Label()
            {
                HorizontalOptions = LayoutOptions.Fill,
                HorizontalTextAlignment = TextAlignment.Center,
                TextColor = Color.Black,
                BackgroundColor = Color.YellowGreen,
                FontSize = Device.GetNamedSize(NamedSize.Large, typeof(Label)),
                Text = "Bocek Keyif Olcer"
            };

            caption.SetBinding(Label.BackgroundColorProperty, "Alarm", 
                BindingMode.Default, new AlarmColorConverter());

            Label rawdata = new Label()
            {
                HorizontalOptions = LayoutOptions.Fill,
                HorizontalTextAlignment = TextAlignment.Center,
                TextColor = Color.Black,
            };
            rawdata.SetBinding(Label.TextProperty, "RawData");


            Label tempCaption = new Label()
            {
                HorizontalOptions = LayoutOptions.Fill,
                HorizontalTextAlignment = TextAlignment.Center,
                TextColor = Color.Black,
                Text = "Sicaklik",
                FontSize = Device.GetNamedSize(NamedSize.Small, typeof(Label))
            };
            Label temp = new Label()
            {
                HorizontalOptions = LayoutOptions.Center,
                VerticalOptions = LayoutOptions.Center,
                TextColor = Color.Black,
                FontSize = 50
            };
            temp.SetBinding(Label.TextProperty, "Temperature");

            Label humCaption = new Label()
            {
                HorizontalOptions = LayoutOptions.Fill,
                HorizontalTextAlignment = TextAlignment.Center,
                TextColor = Color.Black,
                Text = "Nem",
                FontSize = Device.GetNamedSize(NamedSize.Small, typeof(Label))
            };
            Label hum = new Label()
            {
                HorizontalOptions = LayoutOptions.Center,
                VerticalOptions = LayoutOptions.Center,
                TextColor = Color.Black,
                FontSize = 50
            };
            hum.SetBinding(Label.TextProperty, "Humidity");

            Grid values = new Grid()
            {
                Margin = new Thickness(10, 10, 10, 10),
                ColumnDefinitions = {
                    new ColumnDefinition() { },
                    new ColumnDefinition() { },
                },
                RowDefinitions = {
                    new RowDefinition() { Height = GridLength.Auto },
                    new RowDefinition() { Height = GridLength.Auto },
                }
            };

            BoxView b = new BoxView()
            {
                BackgroundColor = Color.LightYellow,
                HeightRequest = 70
            };

            values.Children.Add(tempCaption, 0, 0);
            values.Children.Add(b, 0, 1);
            values.Children.Add(temp, 0, 1);
            values.Children.Add(humCaption, 1, 0);
            values.Children.Add(b, 1, 1);
            values.Children.Add(hum, 1, 1);


            this.Content = new StackLayout()
            {
                Children =
                {
                    caption, 
                    values, 
                    rawdata
                }
            };
           

        }


    }
}
