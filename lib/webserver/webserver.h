#ifndef __WEBSERVER_H
#define __WEBSERVER_H

#include <Arduino.h>
#pragma once
namespace WebServer {

  void setJsonData(char jsonData[256]);

  void pageNotFound(AsyncWebServerRequest *request);

  class wserver {
    public:
      void webserver();
  };

  #endif

  /*
  #ifndef __WEBSERVER_H
  #define __WEBSERVER_H

  #include <Arduino.h>
  // pragma once prevents the file to be included twice
  #pragma once

  extern String jsondata;
  extern char json_string[500];

  void notFound(AsyncWebServerRequest *request);
  void webserver(AsyncWebServer server);

  #endif
  */
}
