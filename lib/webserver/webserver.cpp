#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncTCP.h>
#include "LittleFS.h"
#include "webserver.h"

AsyncWebServer server(80);

void pageNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void wserver::webserver() {
    LittleFS.begin();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/index.html", String(), false);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", String(millis()));
    });

    server.onNotFound(pageNotFound);

    server.begin();
    Serial.println("HTTP server started");
}
