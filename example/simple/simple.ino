#include <WiFi.h>
#include <ConfigPortal.h>
#include "schema.h"

ConfigPortal portal(CONFIG_SCHEMA, "/ConfigPortal/config.json");

void setup() {
  Serial.begin(115200);
  
  // Start WiFi in AP mode for configuration
  WiFi.softAP("ESP32-Config");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  
  portal.begin();
}

void loop() {
  // Access configuration values
  String ssid = portal.get("st_ssid");
  bool debug = portal.get("debug") == "true";
  int refreshRate = portal.get("refresh_rate").toInt();
  
  if(!ssid.isEmpty()) {
    Serial.print("Connecting to: "); Serial.println(ssid);
  }
  
  if(debug) {
    Serial.print("Refresh rate: "); Serial.println(refreshRate);
  }
  
  delay(5000);
}