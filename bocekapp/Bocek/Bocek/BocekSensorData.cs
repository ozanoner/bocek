using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text;

namespace Bocek
{
    public class BocekSensorData : INotifyPropertyChanged
    {
        protected float humidity = 0;
        [JsonProperty("hum")]
        public float Humidity
        {
            get { return this.humidity; }
            set
            {
                if (this.humidity != value)
                {
                    this.humidity = value;
                    this.NotifyPropertyChanged();
                }
            }
        }
        protected float temperature;
        [JsonProperty("temp")]
        public float Temperature
        {
            get { return this.temperature; }
            set
            {
                if (this.temperature != value)
                {
                    this.temperature = value;
                    this.NotifyPropertyChanged();
                }
            }
        }
        protected bool alarm;
        [JsonProperty("alarm")]
        public bool Alarm
        {
            get { return this.alarm; }
            set
            {
                if (this.alarm != value)
                {
                    this.alarm = value;
                    this.NotifyPropertyChanged();
                }
            }
        }


        //protected bool isValid;
        //[JsonProperty("err")]
        //public bool IsValid
        //{
        //    get { return this.isValid; }
        //    set
        //    {
        //        if (this.isValid != value)
        //        {
        //            this.isValid = value;
        //            this.NotifyPropertyChanged();
        //        }
        //    }
        //}
        protected string rawData;
        public string RawData
        {
            get { return this.rawData; }
            set
            {
                if (this.rawData != value)
                {
                    this.rawData = value;
                    this.NotifyPropertyChanged();

                    var def = new { err = false };
                    var preview = JsonConvert.DeserializeAnonymousType(value, def);
                    if(!preview.err)
                        JsonConvert.PopulateObject(value, this);
                }
            }
        }

        public DateTime UpdateTime { get; protected set; }

        public bool DisconnectedAlarm { get; protected set; }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            this.UpdateTime = DateTime.Now;
        }

    }
}
