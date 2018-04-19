/*
https://pubsubclient.knolleary.net/index.html

*/


#include <ESP8266WiFi.h>
#include <Adafruit_BME280.h>
#include <PubSubClient.h>



WiFiClient wifi;
PubSubClient mqtt(wifi);

const char* SSID = "ozanopo";
const char* SSID_PWD = "";

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
    Serial.begin(115200);
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PWD);
    Serial.print("\nWiFi connecting ");
    status = StatusEnum::BOCEK_WIFI_PENDING;

    mqtt.setServer(MQTT_SRV, MQTT_PORT); 
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
                    Serial.print(".");
                }
            }
        break;

        case StatusEnum::BOCEK_READY:
            if(mqtt.connected()) {
                mqtt.loop();

                if(nextLoop<millis()) {
                    // TODO: read bme280
                    Serial.print("bme280 read\n");
                    if(!mqtt.publish("bocek", "{\"temp\":25, \"hum\":50, \"alarm\":false}")) {
                        Serial.print("err: publish failed\n");
                    }
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

/*
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid     = "your-ssid";
const char* password = "your-password";

#define mqtt_server "ADRESSE-IP-MOSQUITTO"
#define mqtt_user "guest"  
#define mqtt_password "guest" 

#define temperature_topic "bocek/temp"  
#define humidity_topic "bocek/hum"    
#define 

WiFiClient espClient;
PubSubClient client(espClient);

void printValues();

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    Serial.begin(115200);

    delay(10);

    bool status;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

    client.setServer(mqtt_server, 1883); 
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("connecting mqtt ...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("error: ");
      Serial.print(client.state());
      Serial.println(" will try in 5 secs");
      delay(5000);
    }
  }
}

void loop() { 
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    printValues();


    delay(delayTime);
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

*/