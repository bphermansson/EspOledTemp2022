#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <mqtt.h>
#include "../../settings.h"

extern  WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void mqtt_connect() {
  #ifdef DEBUG
    Serial.println("Connecting to MQTT");
  #endif

  if (!mqttClient.connected()) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    while (!mqttClient.connected()) {
      #ifdef DEBUG
        Serial.print("Attempting MQTT connection...");
      #endif

      String mqttClientId = APPNAME;
      if (mqttClient.connect(mqttClientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
        #ifdef DEBUG
          Serial.println("connected");
        #endif
      } else {
        #ifdef DEBUG
          Serial.print("failed, rc=");
          Serial.print(mqttClient.state());
          Serial.println(" will try again in 5 seconds");
        #endif
        delay(5000);
      }
    }
  }
  else {
      #ifdef DEBUG
        Serial.println("Already connected");
      #endif
  }
}

void mqtt_publish(char *mess) {
  #ifdef DEBUG
    Serial.println("Publish");
  #endif
  mqttClient.publish(MQTT_PUB_TOPIC,mess);
  mqttClient.disconnect(); // Disconnect makes sure the message is published before going to sleep.
}
void mqtt_loop(){
      mqttClient.loop();
}
