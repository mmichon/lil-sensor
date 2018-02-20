# YAWEWSPTM

Yet Another Web-Enabled Weather Sensor Publishing To MQTT

This is ESP-8266 code that reads temp and humidity readings every minute from a DHT11, updates a small SSD-1306-based display, and publishes readings to MQTT topics. The hardware should cost <$15 and wiring it up should take <15 minutes.

<a href="https://www.youtube.com/embed/H_7alrjquLI" target="_blank"><img src="http://i.imgur.com/WvYhmjtl.jpeg"
alt="Video" height="250" border="10" /><p>Demo Video</a>


![temp](http://i.imgur.com/yK4GflIl.png)
![humid](http://i.imgur.com/gZP5Fbcl.png)

## Hardware
All this should cost <$15
* ESP8266 microcontroller, like a [Wemos D1 Mini](https://www.aliexpress.com/store/product/WEMOS-D1-mini-Pro-16M-bytes-external-antenna-connector-ESP8266-WIFI-Internet-of-Things-development-board/1331105_32724692514.html)
* DHT11 temp/humidity sensor, like the [Wemos shield](https://www.wemos.cc/product/dht-shield.html)
* SSD1036 OLED display, like a [Wemos OLED Shield](https://www.wemos.cc/product/oled-shield.html)
* Wifi connection

## Software Dependencies
* platformio
* The following libs:
```
Adafruit DHT Unified_ID18
DHT sensor library_ID19
Adafruit Unified Sensor_ID31
Adafruit GFX Library_ID13
Adafruit SSD1306_135
PubSubClient_ID89
```
These libraries are loaded automatically by platformio upon ```lib_deps``` dependencies. No need for manual install.
If you use atom.io yoou should open this project with the platformio IDE to get these libraries into the list of installed libraries.

## Provisioning an MQTT topic and connecting to Wifi
1. Create an account on devicehub.net
1. Create a project, device, and 2 analog 'sensors' (```temp``` and ```humidity```)
1. Jot down your API key, project ID, device UUID, and sensor names
1. Change ```config.h``` to match your Wifi and MQTT settings
