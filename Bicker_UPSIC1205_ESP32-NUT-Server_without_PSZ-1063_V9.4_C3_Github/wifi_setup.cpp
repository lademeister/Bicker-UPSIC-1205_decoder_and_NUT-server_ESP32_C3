#include "wifi_setup.h"



void WiFiManagerESP::begin()
{

  Serial.println();
  Serial.println("Connecting WiFi...");
  

  WiFi.mode(WIFI_STA);


  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );


  uint8_t timeout = 0;


  while(WiFi.status()!=WL_CONNECTED)
  {

    delay(500);

    Serial.print(".");


    timeout++;


    if(timeout > 40)
    {

      Serial.println();
      Serial.println("WiFi failed");

      return;

    }

  }



  connected=true;


  Serial.println();
  Serial.println("WiFi connected");


  Serial.print("IP address: ");

  Serial.println(
    WiFi.localIP()
  );


  Serial.print("Hostname: ");

  Serial.println(
    WIFI_HOSTNAME
  );


  WiFi.setHostname(
    WIFI_HOSTNAME
  );


}




void WiFiManagerESP::loop()
{


  if(
    WiFi.status()!=WL_CONNECTED &&
    connected
  )
  {

    connected=false;

    Serial.println(
      "WiFi lost"
    );

  }



  if(
    WiFi.status()==WL_CONNECTED &&
    !connected
  )
  {

    connected=true;


    Serial.print(
      "WiFi restored IP="
    );


    Serial.println(
      WiFi.localIP()
    );


  }


}




String WiFiManagerESP::getIP()
{

  if(WiFi.status()==WL_CONNECTED)
  {

    return WiFi.localIP().toString();

  }


  return "0.0.0.0";

}
