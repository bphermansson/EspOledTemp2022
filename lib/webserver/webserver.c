#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncTCP.h>


void webserver() {
    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
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
  