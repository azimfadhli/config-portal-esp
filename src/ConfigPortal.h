#ifndef CONFIG_PORTAL_H
#define CONFIG_PORTAL_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>

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