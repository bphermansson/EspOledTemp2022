#pragma once
#ifndef MQTT_H_INCLUDED
#define MQTT_H_INCLUDED
    void mqtt_connect();
    void mqtt_publish(char *topic, char *mess);
    void mqtt_loop();
#endif