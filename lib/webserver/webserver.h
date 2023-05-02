#ifndef __WEBSERVER_H
#define __WEBSERVER_H

#include <Arduino.h>
#pragma once

void pageNotFound(AsyncWebServerRequest *request);

class wserver {
  public:
    void webserver();
};

#endif
