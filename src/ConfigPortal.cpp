#include "ConfigPortal.h"

// Constructor with default path
ConfigPortal::ConfigPortal(const char* schema, const char* configPath)
  : _schema(schema), _configPath(configPath) {
  _config = JsonDocument();
}

void ConfigPortal::begin() {
  
    Serial.println("");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  loadConfig();

  _server.on("/schema", HTTP_GET, [this](AsyncWebServerRequest* request) {
    handleGetSchema(request);
  });

  _server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
    servePortal(request);
  });

  _server.on("/set", HTTP_GET, [this](AsyncWebServerRequest* request) {
    handleSetConfig(request);
  });

  _server.on("/restart", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", "ESP Restarting...");
    //delay(1000);
    ESP.restart();
  });

  _server.begin();
}

void ConfigPortal::loadConfig() {
  if (LittleFS.exists(_configPath)) {
    File file = LittleFS.open(_configPath, "r");
    if (file) {
      DeserializationError error = deserializeJson(_config, file);
      if (error) {
        Serial.print("Failed to parse config file: ");
        Serial.println(error.c_str());
      }
      file.close();
    }
  } else {
    Serial.println("Config file not found, will create new one");
  }
}

void ConfigPortal::saveConfig() {
  File file = LittleFS.open(_configPath, "w");
  if (file) {
    serializeJson(_config, file);
    file.close();
    Serial.print("Config saved to: ");
    Serial.println(_configPath);
  } else {
    Serial.println("Failed to open config file for writing");
  }
}

String ConfigPortal::get(const String& key) {
  if (!_config[key].isNull()) {
    return _config[key].as<String>();
  }
  return "";
}

void ConfigPortal::replaceMacros(String& content) {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  String macId = String(mac[0], HEX) + String(mac[1], HEX) + String(mac[2], HEX) + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);
  macId.toUpperCase();
  content.replace("{mac}", macId);
}

String ConfigPortal::processor(const String& var) {
  if (!_config[var].isNull()) {
    return _config[var].as<String>();
  }
  return "";
}

void ConfigPortal::handleSetConfig(AsyncWebServerRequest* request) {
  String response = "OK";

  for (int i = 0; i < request->params(); i++) {
    const AsyncWebParameter* p = request->getParam(i);
    if (p->name() && p->value()) {
      _config[p->name()] = p->value();
      response = p->value();
    }
  }

  saveConfig();
  request->send(200, "text/plain", response);
}

void ConfigPortal::handleGetSchema(AsyncWebServerRequest* request) {
  // Create a JSON response with pretty formatting
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  
  // Copy schema from PROGMEM to RAM
  String schema(_schema);
  
  // Format if valid JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, schema);
  
  if(!error) {
    serializeJsonPretty(doc, *response);
  } else {
    // If not valid JSON, send as-is
    response->print(_schema);
    Serial.print("Schema is not valid JSON: ");
    Serial.println(error.c_str());
  }
  
  request->send(response);
}

void ConfigPortal::servePortal(AsyncWebServerRequest* request) {
  // Generate HTML similar to your example
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Configuration</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f0f4f8;
      color: #333;
      text-align: center;
      padding: 20px;
    }
    h1 {
      color: #4CAF50;
    }
    .form-container {
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.1);
      padding: 20px;
      max-width: 600px;
      margin: 0 auto;
    }
    .form-container input[type="text"], 
    .form-container input[type="password"], 
    .form-container input[type="submit"], 
    .form-container textarea {
      width: 100%;
      padding: 10px;
      margin: 10px 0;
      border: 1px solid #ccc;
      border-radius: 5px;
      box-sizing: border-box;
      color: #888;
      font-weight: normal;
    }
    .form-container input[type="text"]:focus, 
    .form-container input[type="password"]:focus, 
    .form-container textarea:focus {
      outline: none;
    }
    .form-container input[type="text"]:not(:placeholder-shown), 
    .form-container input[type="password"]:not(:placeholder-shown), 
    .form-container textarea:not(:placeholder-shown) {
      color: #333;
      font-weight: bold;
    }
    .form-container input[type="submit"] {
      background-color: #4CAF50;
      color: white;
      cursor: pointer;
    }
    .form-container input[type="submit"]:hover {
      background-color: #45a049;
    }
    .form-container .restart-button {
      background-color: #f44336;
      color: white;
      cursor: pointer;
      border-radius: 5px;
      padding: 8px 20px;
      width: 100%;
    }
    .form-container .restart-button:hover {
      background-color: #d32f2f;
    }
    .form-container label {
      font-size: 1.1em;
      margin-bottom: 10px;
      display: block;
      text-align: center;
    }
    .form-container .form-title {
      font-size: 1.5em;
      color: #4CAF50;
      margin-bottom: 20px;
    }
    .form-container .current-value {
      font-size: 1em;
      color: #888;
      margin-top: 5px;
    }
    .footer {
      margin-top: 40px;
      font-size: 0.9em;
      color: #777;
    }
    .form-container textarea {
      height: 100px;
      resize: vertical;
      font-family: Arial, sans-serif;
      font-size: 1em;
    }
  </style>
  <script>
    function submitMessage() {
      alert("Saved value to ESP LittleFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }

    function restartESP() {
      alert("Restarting ESP...");
      setTimeout(function(){ window.location.href = "/restart"; }, 500);
    }
  </script>
</head>
<body>
  <h1>ESP Configuration Page</h1>
  <div class="form-container">
    <div class="form-title">Enter Configuration Details</div>
  )rawliteral";

  JsonDocument schemaDoc;
  // Copy schema from PROGMEM to RAM
  String schema(_schema);
  deserializeJson(schemaDoc, schema);
  JsonArray sections = schemaDoc.as<JsonArray>();

  for (JsonObject section : sections) {
    html += "<fieldset><legend>" + section["section"].as<String>() + "</legend>";

    JsonArray vars = section["vars"];
    for (JsonObject var : vars) {
      String name = var["name"].as<String>();
      String label = var["label"].as<String>();
      String type = var["type"].as<String>();
      String defaultValue = var["default"].as<String>();
      String currentValue = processor(name);

      if (currentValue == "") currentValue = defaultValue;

      html += "<form action=\"/set\" target=\"hidden-form\">";
      html += "<label for=\"" + name + "\">" + label + "</label>";
      html += "<div class=\"current-value\">Current value: " + currentValue + "</div>";

      if (type == "select") {
        html += "<select name=\"" + name + "\" id=\"" + name + "\">";
        JsonArray options = var["options"];
        for (JsonObject option : options) {
          String optValue = option["value"].as<String>();
          String optLabel = option["label"].as<String>();
          html += "<option value=\"" + optValue + "\"";
          if (optValue == currentValue) html += " selected";
          html += ">" + optLabel + "</option>";
        }
        html += "</select>";
      } else if (type == "checkbox") {
        html += "<input type=\"checkbox\" name=\"" + name + "\" id=\"" + name + "\" value=\"true\"";
        if (currentValue == "true") html += " checked";
        html += ">";
      } else if (type == "textarea") {
        html += "<textarea name=\"" + name + "\" id=\"" + name + "\" placeholder=\"Enter " + label + "\">" + currentValue + "</textarea>";
      } else {
        String attribs = var.containsKey("attribs") ? var["attribs"].as<String>() : "";
        html += "<input type=\"" + type + "\" name=\"" + name + "\" id=\"" + name + "\" value=\"" + currentValue + "\" placeholder=\"Enter " + label + "\" " + attribs + ">";
      }

      html += "<input type=\"submit\" value=\"Update " + name + "\" onclick=\"submitMessage()\">";
      html += "</form><br>";
    }
    html += "</fieldset>";
  }

  html += R"rawliteral(
    <button class="restart-button" onclick="restartESP()">Restart ESP</button>
  </div>

  <iframe style="display:none" name="hidden-form"></iframe>

  <div class="footer">© 2025 ESP Configuration Portal</div>
</body>
</html>
  )rawliteral";
 
  //replaceMacros(html);

  Serial.println(html.length() );
  request->send(200, "text/html", html);
}