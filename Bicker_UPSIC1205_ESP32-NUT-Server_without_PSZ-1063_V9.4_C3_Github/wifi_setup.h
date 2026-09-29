#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"


class WiFiManagerESP
{

public:

  void begin();


  void loop();


  String getIP();



private:

  bool connected = false;


};


#endif
