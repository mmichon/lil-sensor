#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>   // Adafruit SSD1306_135 --> local lib/
#include <Adafruit_GFX.h>       // Adafruit GFX Library_ID13
#include <DHT_U.h>              // Adafruit DHT Unified_ID18, DHT sensor library_ID19, Adafruit Unified Sensor_ID31
#include <PubSubClient.h>       // PubSubClient_ID89
#include "config.h"


// Wemos OLED shield on pin 3: https://www.wemos.cc/product/oled-shield.html
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// change the #define in Adafruit_SSD1306.h if you're using a different display size
#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
    Serial.begin(9600);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

    // clear the display
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Hello.");
    display.display();

    // warm up the temp sensor
    dht.begin();

    // get on wifi
    setup_wifi();

    // connect to mqtt
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    float h_val = dht.readHumidity();

    // farheneit reading
    float t_val = dht.readTemperature(true);

    // check if any reads failed and exit early
    if (isnan(h_val) || isnan(t_val)) {
        Serial.println("Failed to read from DHT sensor!");

        // pause a couple seconds before the next reading
        delay(2000);

        return;
    }

    // convert the floats to strings and round to 2 decimal places
    char h_str[10], t_str[10];
    dtostrf(h_val, 1, 1, h_str);
    dtostrf(t_val, 1, 1, t_str);

    // clear display before updating
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    // output readings
    display.setTextSize(1);
    display.println("temp:");
    display.setTextSize(2);
    display.print(t_str);
    display.println("F");

    display.setTextSize(1);
    display.println("humid:");
    display.setTextSize(2);
    display.print(h_str);
    display.println('%');

    display.display();

    // publish to mqtt
    char topic[150];
    char payload[20];

    sprintf(topic, "%s/%s/data", mqtt_topic_prefix, "temp");
    sprintf(payload, "{\"value\":%s}", t_str);

    Serial.println(topic);
    Serial.println(payload);

    if (!client.publish(topic, payload, true)){
        Serial.println("ERROR: couldn't publish.");
    }

    sprintf(topic, "%s/%s/data", mqtt_topic_prefix, "humidity");
    sprintf(payload, "{\"value\":%s}", h_str);

    Serial.println(topic);
    Serial.println(payload);

    if (!client.publish(topic, payload, true)){
        Serial.println("ERROR: couldn't publish.");
    }

    // wait for a minute before publishing again
    delay(60*1000);
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        display.print(".");
        display.display();
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println();
        Serial.print("Reconnecting");

        WiFi.begin(ssid, password);

        delay(500);
        Serial.print(".");
    }
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection... ");
        if (client.connect("ESP8266_Client")) {
            Serial.println("connected");
        } else {
            // debug MQTT issues on serial if you can't connect
            // check out http://www.hivemq.com/blog/mqtt-essentials-part-3-client-broker-connection-establishment for ack flag meaning
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
