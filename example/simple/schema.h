#pragma once
#include <ArduinoJson.h>

const char CONFIG_SCHEMA[] PROGMEM = R"json([
  {
    "section": "Wifi settings",
    "vars": [
      {
        "name": "st_ssid",
        "label": "Name for WLAN",
        "type": "text",
        "default": ""
      },
      {
        "name": "st_pass",
        "label": "Password for WLAN",
        "type": "password",
        "default": ""
      },
      {
        "name": "host_name",
        "label": "Host name to use for MDNS and AP<br>{mac} will be replaced with device's mac id",
        "type": "text",
        "default": "configAssist_{mac}"
      }
    ]
  },
  {
    "section": "Application settings",
    "vars": [
      {
        "name": "app_name",
        "label": "Name your application",
        "type": "text",
        "default": "ConfigAssistDemo"
      },
      {
        "name": "led_buildin",
        "label": "Enter the pin that the build in led is connected. Leave blank for auto.",
        "type": "number",
        "default": "2",
        "attribs": "min='2' max='23' step='1'"
      }
    ]
  },
  {
    "section": "ConfigAssist settings",
    "vars": [
      {
        "name": "display_style",
        "label": "Choose how the config sections are displayed. Must reboot to apply",
        "type": "select",
        "options": [
          {"label": "AllOpen", "value": "0"},
          {"label": "AllClosed", "value": "1"},
          {"label": "Accordion", "value": "2"},
          {"label": "AccordionToggleClosed", "value": "3"}
        ],
        "default": "3"
      },
      {
        "name": "work_mode",
        "label": "Application Work mode. Must reboot to apply",
        "type": "text",
        "default": "MeasureUploadSleep"
      }
    ]
  },
  {
    "section": "Other settings",
    "vars": [
      {
        "name": "float_val",
        "label": "Enter a float val",
        "type": "number",
        "default": "3.14159",
        "attribs": "min='2.0' max='5' step='.001'"
      },
      {
        "name": "debug",
        "label": "Check to enable debug",
        "type": "checkbox",
        "default": "false"
      },
      {
        "name": "sensor_type",
        "label": "Enter the sensor type",
        "type": "select",
        "options": [
          {"label": "BMP280", "value": "BMP280"},
          {"label": "DHT12", "value": "DHT12"},
          {"label": "DHT21", "value": "DHT21"},
          {"label": "DHT22", "value": "DHT22"}
        ],
        "default": "DHT22"
      },
      {
        "name": "refresh_rate",
        "label": "Enter the sensor update refresh rate",
        "type": "number",
        "default": "30",
        "attribs": "min='10' max='50' step='1'"
      },
      {
        "name": "time_zone",
        "label": "Needs to be a valid time zone string",
        "type": "text",
        "default": "EET-2EEST,M3.5.0/3,M10.5.0/4"
      },
      {
        "name": "cal_data",
        "label": "Enter data for 2 Point calibration.<br>Data will be saved to /calibration.ini",
        "type": "textarea",
        "default": "X1=222, Y1=1.22\nX2=900, Y2=3.24"
      }
    ]
  }
])json";