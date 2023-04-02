#include <Arduino.h>
#include "../settings.h"
#include <print_on_oled.h>
#include "wifi.h"
#include "main.h"

WiFiClient wifiClient;


void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.printf("Welcome to %s!\n", APPNAME);
  #endif

  *ipAddrPtr=0;
  wifi_connect(ipAddrPtr);
  
  initOled();
  char text_to_write_oled[100];
  //strcpy(text_to_write_oled, "A long long test text to test with, now it is even longer to make sure it works");
  char temp[] = APPNAME;
  strcpy(text_to_write_oled, temp);
  clearOled();
  printoled(text_to_write_oled, 8, 32); // Position is at texts lower left edge. Display is 128x64.

  delay(1000);
  strcpy(text_to_write_oled, "RAD1");
  clearOled();
  printoled(text_to_write_oled, 0, 43);
  delay(300);

  strcpy(text_to_write_oled, "RAD2");
  clearOled();
  printoled(text_to_write_oled, 12, 13);
  delay(300);

  strcpy(text_to_write_oled, "RAD3");
  clearOled();
  printoled(text_to_write_oled, 80, 64);
  delay(300);

}

void loop() {
  delay(100);
  yield();
}