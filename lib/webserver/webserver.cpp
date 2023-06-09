#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncTCP.h>
#include "LittleFS.h"

#include "webserver.h"

namespace WebServer {

  AsyncWebServer server(80);

  String _jsonData;

  void setJsonData(char jsonData[256]) {
    _jsonData = jsonData;
  }

  void pageNotFound(AsyncWebServerRequest *request) {
      request->send(404, "text/plain", "Not found");
  }

  void wserver::webserver() {

      LittleFS.begin();

      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("Request");
        request->send(LittleFS, "/index.html", String(), false);
      });
      server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
      });

      server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", _jsonData);
      });

      server.onNotFound(pageNotFound);

      server.begin();
      Serial.println("HTTP server started");
  }


  /*
  Old webserver code

  #include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
  #include <ESPAsyncTCP.h>
  #include "LittleFS.h"

  String jsondata;
  char json_string[500];

  void notFound(AsyncWebServerRequest *request) {
      request->send(404, "text/plain", "Not found");
  }

  void webserver(AsyncWebServer server) {
      // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request");
      request->send(LittleFS, "/index.html", String(), false);
    });
    server.on("/json.html", HTTP_GET, [](AsyncWebServerRequest *request){
    });
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/style.css", "text/css");
    });
    server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", jsondata);
    });
    server.onNotFound(notFound);

    server.begin();
    Serial.println("HTTP server started");
  }
    
  */
}
