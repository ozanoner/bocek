using System;

using Xamarin.Forms;
using System.Threading.Tasks;

using dbg = System.Diagnostics.Debug;
using uPLibrary.Networking.M2Mqtt;
using System.Text;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace Bocek
{
    public partial class App : Application
    {
        MqttClient mqtt;
        BocekSensorData sensorData;

        public App()
        {
            this.mqtt = new MqttClient("34.241.70.227");
            this.mqtt.ConnectionClosed += (s, e) =>
            {
                dbg.WriteLine($"[Bocek-App] mqtt connection closed");
            };

            this.mqtt.Subscribe(new string[] { "bocek" },
                new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
            this.mqtt.MqttMsgPublishReceived += (s, e) =>
            {
                string m = UTF8Encoding.UTF8.GetString(e.Message);
                dbg.WriteLine($"[Bocek-App] mqtt msg({e.Topic}): {m}");
                Device.BeginInvokeOnMainThread(() =>
                {
                    try
                    {
                        this.sensorData.RawData = m;
                    }
                    catch(Exception ex )
                    {
                        dbg.WriteLine($"[Bocek-App] exception {ex.ToString()}");
                    }
                });
            };

            Task.Run(() =>
            {
                var r = this.mqtt.Connect("mobile_app");
                dbg.WriteLine($"[Bocek-App] mqtt connect r: {r}");
            });

            InitializeComponent();

            MainPage = new Bocek.MainPage();
            sensorData = new BocekSensorData();
            this.MainPage.BindingContext = sensorData;

            this.MainPage.BindingContextChanged += (s, e) =>
            {
                dbg.WriteLine($"[Bocek-App] in App. mainpage bindingcontext changed");
            };
        }

        protected override void OnStart()
        {
            // Handle when your app starts
        }

        protected override void OnSleep()
        {
            // Handle when your app sleeps
        }

        protected override void OnResume()
        {
            // Handle when your app resumes
        }
    }
}
