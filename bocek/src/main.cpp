/*
https://pubsubclient.knolleary.net/index.html

*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


#define DHTPIN D3
#define RELAYPIN D6

WiFiClient wifi;
PubSubClient mqtt(wifi);
DHT dht(DHTPIN, DHT22);


const char* SSID = "breakingBad";
const char* SSID_PWD = "@@oy108-33";
// const char* SSID = "ozanopo";
// const char* SSID_PWD = "baa533f161fc";

const char* MQTT_SRV = "34.241.70.227";
const int MQTT_PORT  = 1883;
const char* MQTT_USR = "";
const char* MQTT_PWD = "";


enum class StatusEnum {
    BOCEK_WIFI_PENDING,
    BOCEK_MQTT_PENDING,
    BOCEK_READY
};

StatusEnum status;

void setup() {
    Serial.begin(9600);
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PWD);
    Serial.print("\nWiFi connecting ");
    status = StatusEnum::BOCEK_WIFI_PENDING;

    mqtt.setServer(MQTT_SRV, MQTT_PORT); 

    dht.begin();
    pinMode(RELAYPIN, OUTPUT);
    digitalWrite(RELAYPIN, 0);
}

unsigned long nextLoop;

void loop() {
    delay(10);
    switch(status) {
        case StatusEnum::BOCEK_WIFI_PENDING:
            if(WiFi.status() == WL_CONNECTED) {
                status = StatusEnum::BOCEK_MQTT_PENDING;
                mqtt.connect("BocekClient");
                nextLoop = millis()+5000; // next mqtt connect
                Serial.print("\nMQTT connecting ");
            }
            else {
                if(!(millis()%500))
                    Serial.print(".");
            }
        break;

        case StatusEnum::BOCEK_MQTT_PENDING:
            if(mqtt.connected()) {
                Serial.print("\nReady\n");
                status = StatusEnum::BOCEK_READY;
                nextLoop = millis()+10000; // next bme280 read
            }
            else {
                if(nextLoop < millis()) {
                    mqtt.connect("BocekClient");
                    nextLoop = millis()+5000; // next mqtt connect
                    Serial.print("\nMQTT connecting ");
                }
                else {
                    if(!(millis()%500))
                        Serial.print(".");
                }
            }
        break;

        case StatusEnum::BOCEK_READY:
            if(mqtt.connected()) {
                mqtt.loop();

                if(nextLoop<millis()) {
                    float h = dht.readHumidity();
                    float t = dht.readTemperature();
                    bool err=false;

                    if(isnan(h) || isnan(t)) {
                        Serial.print("dht22 read failed\n");
                        h = t = -1;
                        err = true;
                    }
                    
                    bool a = h>80 || t<23 || t>28;
                    char buff[64];
                    snprintf(buff, 64, "{\"temp\":%.1f, \"hum\":%.1f, \"alarm\":%d, \"err\":%d}", t, h, a, err);
                    if(!mqtt.publish("bocek", buff)) {
                        Serial.print("err: publish failed\n");
                    }
                    else {
                        Serial.print(buff);
                        Serial.print("\n");
                    }

                    if(!err)
                        digitalWrite(RELAYPIN, h>80 || t>28);

                    nextLoop = millis()+10000; // next bme280 read
                }
            }
            else {
                status = StatusEnum::BOCEK_MQTT_PENDING;
                mqtt.connect("BocekClient");
                nextLoop = millis()+5000; // next mqtt connect
                Serial.print("\nMQTT connecting ");
            }
            

        break;

        default:
        break;
    }
}
