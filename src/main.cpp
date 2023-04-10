/**
 * @file htu21d.h
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

#include "../settings.h"
#include <print_on_oled.h>
#include "wifi.h"
#include "mqtt.h"
#include "htu21d.h"
#include "main.h"

WiFiClient wifiClient;
StaticJsonDocument<132> mqtt_json_temp;
char *mqtt_json;
char json_string[300];

int counter = 0;
const int interval = 30000;
int oldcounter = 0;
htuvalues *htuPtr = (htuvalues*)malloc(sizeof(htuvalues));

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.printf("Welcome to %s!\n", APPNAME);
  #endif
  
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

  counter = millis();
  if (counter-oldcounter>interval){
    read_htu(htuPtr); 
    int htu_status = htuPtr->state;
    float htu_temp = htuPtr->temp;
    float htu_humidity = htuPtr->humidity;
    
    char temp_rounded[8], humidity_rounded[5];
   
    sprintf(temp_rounded, "%.02f", htu_temp); 
    sprintf(humidity_rounded, "%.02f", htu_humidity);

    #ifdef DEBUG
      char text[300];
      sprintf(text, "Sensor status: %d - Temp: %lf - Humidity - %s", htu_status, temp_rounded, humidity_rounded);
      Serial.println(text);
    #endif

    mqtt_json_temp["sensor"] = APPNAME;
    mqtt_json_temp["htu_status"] = htu_status;
    mqtt_json_temp["htu_temp"] = htu_temp;
    mqtt_json_temp["htu_humidity"] = humidity_rounded;
 
 
    serializeJson(mqtt_json_temp, json_string);
    mqtt_connect(); 
    mqtt_publish(json_string); 
    #ifdef DEBUG
      Serial.println("----------------");
    #endif
    // Reset counter
    oldcounter=counter;
  }
}