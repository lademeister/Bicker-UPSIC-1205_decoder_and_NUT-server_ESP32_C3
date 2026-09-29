#include "ota.h"



void OTAUpdater::begin()
{

  ArduinoOTA.setHostname(
    OTA_HOSTNAME
  );


  ArduinoOTA
  .onStart([]()
  {

    String type;


    if(
      ArduinoOTA.getCommand()
      ==
      U_FLASH
    )
    {
      type="sketch";
    }
    else
    {
      type="filesystem";
    }


    Serial.println(
      "OTA Start: " + type
    );


  });



  ArduinoOTA
  .onEnd([]()
  {

    Serial.println(
      "\nOTA finished"
    );

  });



  ArduinoOTA
  .onProgress([](
    unsigned int progress,
    unsigned int total
  )
  {

    Serial.printf(
      "OTA Progress: %u%%\r",
      (progress * 100) / total
    );


  });



  ArduinoOTA
  .onError([](
    ota_error_t error
  )
  {

    Serial.printf(
      "OTA Error[%u]: ",
      error
    );


    if(error == OTA_AUTH_ERROR)
      Serial.println("Auth failed");


    else if(error == OTA_BEGIN_ERROR)
      Serial.println("Begin failed");


    else if(error == OTA_CONNECT_ERROR)
      Serial.println("Connect failed");


    else if(error == OTA_RECEIVE_ERROR)
      Serial.println("Receive failed");


    else if(error == OTA_END_ERROR)
      Serial.println("End failed");


  });



  ArduinoOTA.begin();



  Serial.println(
    "OTA ready"
  );


}




void OTAUpdater::loop()
{

  ArduinoOTA.handle();

}
