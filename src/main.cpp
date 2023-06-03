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
#include <Bounce2.h>
#include "main.h"

#include <ESPAsyncTCP.h>

WiFiClient wifiClient;
WebServer::wserver newWebserver;

char *mqtt_json;
char text_to_write_oled[100];

unsigned long sensor_read_interval=300000;    // the time we need to wait
unsigned long clock_update_interval=1000;
int time_period = 1000;
unsigned long time_now = 0;
int check_int=0;

unsigned long previousMillis=0, previousClockMillis=0;
bool firstStart = true; 
htuvalues *htuPtr = (htuvalues*)malloc(sizeof(htuvalues));

mytime_t mytime;

int temp = 0;

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.printf("Welcome to %s!\n", APPNAME);
  #endif

  pinMode(BUTTON_PIN, INPUT_PULLUP);   

  (void)getInternetTime(mytime); 

  initOled();
  strcpy(text_to_write_oled, "Boot"); 
  uint8_t font_to_use=3;
  printoled(text_to_write_oled, font_to_use, 23, 29);
  //strcpy(text_to_write_oled, "A long long test text to test with, now it is even longer to make sure it works");
  char temp[] = APPNAME;
  strcpy(text_to_write_oled, temp); 
  clearOled();
 // printoled(text_to_write_oled, 8, 32); // Position is at texts lower left edge. Display is 128x64.
  delay(800);

  init_htu(htuPtr);

  char *ipAddrPtr = ipAddr;
  wifi_connect(ipAddrPtr);
  #ifdef DEBUG
    Serial.printf("Connected, IP: %s!\n", ipAddrPtr);
  #endif
  
  strcpy(text_to_write_oled, ipAddrPtr); 
  clearOled();
  delay(800);

  (void)getInternetTime(mytime); 

  newWebserver.webserver();

  clearOled();

}

void loop() {
  delay(10);  // <- fixes some issues with WiFi stability
  
  mqtt_loop();

  ArduinoOTA.handle();

  StaticJsonDocument<192> mqtt_json_temp; // This should be in loop(), code works better then.
  char json_topic[20] = MQTT_PUB_BTN_TOPIC;
  char json_string[256];

  temp = digitalRead(BUTTON_PIN);
  if (temp == LOW) {
    #ifdef DEBUG
      Serial.println("Button pressed");
    #endif

  strcpy(text_to_write_oled, "PUSH"); 
  printoled(text_to_write_oled, 2, 15, 36);

  mqtt_json_temp["btn"] = "pressed";
  serializeJson(mqtt_json_temp, json_string);
  mqtt_connect(); 
  mqtt_publish(json_topic, json_string); 
    
  }
  
  if ((unsigned long)(millis() - previousMillis) >= sensor_read_interval || firstStart == true) {
      previousMillis = millis();

    firstStart = false;
    
    #ifdef DEBUG
      Serial.println("Read sensors");
    #endif

    read_htu(htuPtr); 
    int htu_status = htuPtr->state;
    float htu_temp = htuPtr->temp;
    float htu_humidity = htuPtr->humidity;
    
    char temp_rounded[18], humidity_rounded[20];
   
    sprintf(temp_rounded, "%1.1f",htu_temp);
    sprintf(humidity_rounded, "%1.1f", htu_humidity);

    #ifdef DEBUG
      char text[300];
      sprintf(text, "Sensor status: %d - Temp: %s - Humidity - %s", htu_status, temp_rounded, humidity_rounded);
      Serial.println(text);
    #endif

    mqtt_json_temp["sensor"] = APPNAME;
    mqtt_json_temp["status"] = htu_status;
    mqtt_json_temp["temp"] = temp_rounded;
    mqtt_json_temp["humidity"] = humidity_rounded;

    #ifdef DEBUG
      Serial.printf("Date/time:%s\n",mytime.readable_date); 
      Serial.printf("Run time:%ld\n", mytime.cur_timestamp);
      Serial.printf("Epoch time:%ld\n", mytime.raw_time);
      Serial.printf("Time:%s\n", mytime.time);
      Serial.printf("Date:%s\n", mytime.date);
    #endif

    mqtt_json_temp["time"] = mytime.time;
    mqtt_json_temp["date"] = mytime.date;
    mqtt_json_temp["runtime"] = mytime.cur_timestamp;
    mqtt_json_temp["epoch"] = mytime.raw_time;

    //char json_string[256];
    strcpy(json_topic, MQTT_PUB_TOPIC);
    serializeJson(mqtt_json_temp, json_string);
    mqtt_connect(); 
    mqtt_publish(json_topic, json_string); 
    
    WebServer::setJsonData(json_string);

    strcpy(text_to_write_oled, temp_rounded);   
    strcat(text_to_write_oled, "C");   
    uint8_t font_to_use=2;
    printoled(text_to_write_oled, font_to_use, 20, 57);

    strcpy(text_to_write_oled, humidity_rounded);     
    strcat(text_to_write_oled, "%");     
    font_to_use=2;
    printoled(text_to_write_oled, font_to_use, 70, 57);
}

    if ((unsigned long)(millis() - previousClockMillis) >= clock_update_interval) {
      previousClockMillis = millis();

      check_int++;
      mytime.seconds++;
      if(check_int>59) {
        (void)getInternetTime(mytime); 
        check_int=0;
        mytime.seconds=0;
      }
      char curTime[20];
      sprintf(curTime, "%02d:%02d:%02d", mytime.hours, mytime.minutes, mytime.seconds);

      strcpy(text_to_write_oled, mytime.date); 
      uint8_t font_to_use=0;
      printoled(text_to_write_oled, font_to_use, 23, 11);

      strcpy(text_to_write_oled, curTime); 
      font_to_use=1;
      printoled(text_to_write_oled, font_to_use, 15, 36);

      #ifdef DEBUG
        //Serial.print("-----TIME-----");
        //Serial.println(text_to_write_oled);
      #endif
    }

}