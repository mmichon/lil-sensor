# YAWEWSPTM

Yet Another Web-Enabled Weather Sensor Publishing To MQTT

This is ESP-8266 code that reads temp and humidity readings every minute from a DHT11, updates a small SSD-1306-based display, and publishes readings to MQTT topics.

<a href="https://www.youtube.com/embed/H_7alrjquLI" target="_blank"><img src="http://i.imgur.com/WvYhmjtl.jpeg" 
alt="Video" height="250" border="10" /><p>Demo Video</a>

## Hardware
* ESP8266 microcontroller, like a Wemos D1 Mini
* DHT11 temp/humidity sensor
* SSD1036 OLED display, like a Wemos OLED Shield
* Wifi connection

## Software Dependencies
* platformio
* The following libs:
```
pio lib install 135
pio lib install 188
pio lib install 18
pio lib install 89
```

## Provisioning an MQTT topic and connecting to Wifi
1. Create an account on devicehub.net
1. Create a project, device, and 2 analog 'sensors' (```temp``` and ```humidity```)
1. Jot down your API key, project ID, device UUID, and sensor names
1. Change ```config.h``` to match your Wifi and MQTT settings