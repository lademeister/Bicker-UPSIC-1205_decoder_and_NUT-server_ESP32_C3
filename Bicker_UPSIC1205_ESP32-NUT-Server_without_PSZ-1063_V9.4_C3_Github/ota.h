#ifndef OTA_H
#define OTA_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "config.h"


class OTAUpdater
{

public:

  void begin();

  void loop();


};


#endif
