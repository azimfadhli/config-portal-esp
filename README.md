# ESP32 ConfigPortal

A lightweight, schema-based configuration portal for ESP32 and ESP8266 boards. It provides a responsive and customizable web interface to manage device settings in real time. Configurations are defined using a simple JSON schema and persistently stored using LittleFS. Ideal for IoT projects that require runtime configurability with minimal code overhead.

## Features

* Responsive Web Interface served from the ESP32
* JSON Schema-based Form Generation
* Persistent Settings saved on LittleFS in /config.json
* Live Configuration Update without requiring reboots (where applicable)
* Auto MAC Replacement for dynamic fields like hostname
* Built-in config types: text, password, number, select, checkbox, textarea
* Inline validation via HTML attributes (e.g. min/max)
* Password fields hidden by default
* Optional reboot via /restart endpoint
* Config schema served over /schema

<img width="661" height="845" alt="Screenshot 2025-08-07 151257" src="https://github.com/user-attachments/assets/f7f24bfb-dc51-4f3b-87d0-e14131122cfc" />

## Platform Support

| Platform | Status    |
| -------- | --------- |
| ESP32    | Supported |
| ESP8266  | Supported |

## Installation

Install the following libraries via Arduino Library Manager:

* ESPAsyncWebServer
* AsyncTCP
* ArduinoJson
* LittleFS (native to ESP32 boards)

LittleFS must be selected as your filesystem in the Arduino Tools menu.

## Getting Started

Basic Example:
```cpp

#include "ConfigPortal.h"
#include "schema.h"

ConfigPortal portal(CONFIG_SCHEMA);
void setup() {
Serial.begin(115200);
WiFi.begin("yourSSID", "yourPASS");
portal.begin();  // Launch config portal
}
void loop() {}
```
The web portal will be available at your ESP's IP address once connected to Wi-Fi.

## Configuration Schema

You define your device configuration in a simple JSON schema. Example snippet:
```JSON
[{
  "section": "Wifi settings",
  "vars": [
    {
      "name": "st_ssid",
      "label": "Name for WLAN",
      "type": "text",
      "default": ""
    }
  ]
}]
```
Supported Types:

| Type     | Description                   |
| -------- | ----------------------------- |
| text     | Single-line input             |
| password | Hidden characters             |
| number   | Numeric input with validation |
| select   | Dropdown with options         |
| checkbox | Toggle true/false             |
| textarea | Multiline input               |

Special Keys:

* attribs: Custom HTML attributes (e.g. min, max, step)
* {mac}: Will be replaced with the device’s MAC address

## Web Interface

Access via: http\://<ESP-IP>/

Available Routes:

| Endpoint | Purpose                             |
| -------- | ----------------------------------- |
| /        | Main configuration portal (HTML UI) |
| /schema  | Returns raw JSON schema             |
| /set     | Set config values via GET request   |
| /restart | Reboots the ESP device              |

## How It Works

* On startup, the portal loads config from /config.json in LittleFS
* Web UI is dynamically generated from the JSON schema
* Submitting a form saves values back to LittleFS and reflects changes immediately
* Macros like {mac} are replaced during rendering
* Schema is stored in PROGMEM for memory efficiency

## Advanced Usage

* Modify schema.h to add/remove config sections or variables
* Customize input validation using attribs
* Add new endpoints or styles by editing ConfigPortal::servePortal()

## File Structure

| File               | Description                                           |
| ------------------ | ----------------------------------------------------- |
| ConfigPortal.h/cpp | Core logic for serving the portal and managing config |
| schema.h           | JSON schema definition stored in PROGMEM              |
| simple.ino         | Example sketch showing basic usage                    |

## Roadmap

* ESP8266 support
* OTA update integration
* Auth-protected admin access
* UI themes (light/dark)
* Backup/restore config support

## License

MIT License. Use it freely in personal and commercial projects.

## Credits

Built with love using open source libraries like ESPAsyncWebServer and ArduinoJson.
