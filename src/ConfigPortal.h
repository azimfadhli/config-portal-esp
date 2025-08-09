#ifndef CONFIG_PORTAL_H
#define CONFIG_PORTAL_H

//#include <Arduino.h>

// Conditional inclusion for Async TCP and WiFi libraries
#ifdef ESP32
  #include <AsyncTCP.h>
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESPAsyncTCP.h>
  #include <ESP8266WiFi.h>
#endif

// Common libraries for both platforms
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class ConfigPortal {
public:
  ConfigPortal(const char* schema, const char* configPath = "/config.json");
  void begin();
  String get(const String& key);
  
private:
  AsyncWebServer _server{80};
  JsonDocument _config;
  const char* _schema;
  const char* _configPath;
  
  void loadConfig();
  void saveConfig();
  void replaceMacros(String& content);
  void handleSetConfig(AsyncWebServerRequest* request);
  void handleGetSchema(AsyncWebServerRequest* request); // New handler
  void servePortal(AsyncWebServerRequest* request);
  String processor(const String& var);
};

#endif