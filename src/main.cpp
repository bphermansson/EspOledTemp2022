/**
 * @file main.cpp
 * @author Patrik Hermansson (hermansson.patrik@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright (c) Paheco 2023
 * 
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer

#include "../settings.h"
#include <print_on_oled.h>
#include "wifi.h"
#include "mqtt.h"
#include "htu21d.h"
#include "internetTime.h"
#include "webserver.h"
#include "main.h"

#include <ESPAsyncTCP.h>
//AsyncWebServer server(80);

WiFiClient wifiClient;
wserver newWebserver;

char *mqtt_json;

unsigned long interval=300000;    // the time we need to wait
unsigned long previousMillis=0; 
htuvalues *htuPtr = (htuvalues*)malloc(sizeof(htuvalues));

mytime_t mytime;

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.printf("Welcome to %s!\n", APPNAME);
  #endif
  
  (void)getInternetTime(mytime); 

  initOled();
  char text_to_write_oled[100];
  //strcpy(text_to_write_oled, "A long long test text to test with, now it is even longer to make sure it works");
  char temp[] = APPNAME;
  strcpy(text_to_write_oled, temp); 
  clearOled();
  printoled(text_to_write_oled, 8, 32); // Position is at texts lower left edge. Display is 128x64.
  delay(800);

  init_htu(htuPtr);

  char *ipAddrPtr = ipAddr;
  wifi_connect(ipAddrPtr);
  #ifdef DEBUG
    Serial.printf("Connected, IP: %s!\n", ipAddrPtr);
  #endif
  
  strcpy(text_to_write_oled, ipAddrPtr); 
  clearOled();
  printoled(text_to_write_oled, 8, 32);
  delay(800);

  (void)getInternetTime(mytime); 

  //(void)webserver(server);
  newWebserver.webserver();

/*
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Request");
request->send(200, "text/plain", "Hello World!");

  });
  
  server.begin();
*/

/*
  strcpy(text_to_write_oled, "TOP LEFT");
  clearOled();
  printoled(text_to_write_oled, 2, 10);
  delay(300);

  strcpy(text_to_write_oled, "RAD2");
  clearOled();
  printoled(text_to_write_oled, 12, 13);
  delay(300);

  strcpy(text_to_write_oled, "Lower Right");
  clearOled();
  int x = strlen(text_to_write_oled) * 8;
  x = 128-x;

  printoled(text_to_write_oled, x, 64);
  delay(300);
*/
}

void loop() {
  
  mqtt_loop();

  ArduinoOTA.handle();

  delay(10);  // <- fixes some issues with WiFi stability
  if ((unsigned long)(millis() - previousMillis) >= interval) {
    previousMillis = millis();
    Serial.println("Read sensors");

    read_htu(htuPtr); 
    int htu_status = htuPtr->state;
    float htu_temp = htuPtr->temp;
    float htu_humidity = htuPtr->humidity;
    
    char temp_rounded[18], humidity_rounded[20];
   
    sprintf(temp_rounded, "%.02f", htu_temp); 
    sprintf(humidity_rounded, "%.02f", htu_humidity);

    #ifdef DEBUG
      char text[300];
      sprintf(text, "Sensor status: %d - Temp: %s - Humidity - %s", htu_status, temp_rounded, humidity_rounded);
      Serial.println(text);
    #endif

    StaticJsonDocument<192> mqtt_json_temp; // This should be in loop(), code works better then.

    mqtt_json_temp["sensor"] = APPNAME;
    mqtt_json_temp["htu_status"] = htu_status;
    mqtt_json_temp["htu_temp"] = temp_rounded;
    mqtt_json_temp["htu_humidity"] = humidity_rounded;

    #ifdef DEBUG
      Serial.printf("Date/time:%s\n",mytime.readable_date); 
      Serial.printf("Run time:%ld\n", mytime.cur_timestamp);
      Serial.printf("Epoch time:%ld\n", mytime.raw_time);
      Serial.printf("Time:%s\n", mytime.time);
      Serial.printf("Date:%s\n", mytime.date);
    #endif

    mqtt_json_temp["readable_time"] = mytime.readable_date;
    mqtt_json_temp["runtime"] = mytime.cur_timestamp;
    mqtt_json_temp["epoch"] = mytime.raw_time;

    char json_string[256];
    serializeJson(mqtt_json_temp, json_string);
    mqtt_connect(); 
    mqtt_publish(json_string); 

  /*
  strcpy(text_to_write_oled, mytime.); 
  clearOled();
  printoled(text_to_write_oled, 8, 8);
*/
/*
    #ifdef DEBUG
      Serial.println("----------------");
    #endif
  }
  */
}}