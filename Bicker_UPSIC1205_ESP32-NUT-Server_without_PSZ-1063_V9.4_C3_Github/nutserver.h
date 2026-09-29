//#ifndef NUTSERVER_H
//#define NUTSERVER_H
//
//#include <Arduino.h>
//#include <WiFi.h>
//#include "config.h"
//#include "powercap.h"
//#include "runtime.h"
//
//
//class NUTServer
//{
//
//public:
//
//  void begin();
//  //String getStatus();
//
//  void handle();
//
//  void update(
//    PowerCapData data,
//    RuntimeEstimator runtime
//  );
//
//
//private:
//  String getStatus();
//  
//  WiFiServer server = WiFiServer(NUT_PORT);
//
//  
//  PowerCapData upsData;
//
//
//  uint32_t runtimeSeconds;
//
//
//  void handleClient(WiFiClient &client);
//
//
//  void sendLine(
//    WiFiClient &client,
//    String text
//  );
//
//
//  void processCommand(
//    WiFiClient &client,
//    String cmd
//  );
//
//
//};
//
//
//
//#endif

#ifndef NUTSERVER_H
#define NUTSERVER_H

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "powercap.h"
#include "runtime.h"

class NUTServer
{

public:

  void begin();

  void handle();

  void update(
    PowerCapData data,
    RuntimeEstimator runtime
  );

  String getStatus();

private:

  //String getStatus();

  WiFiServer server = WiFiServer(NUT_PORT);

  PowerCapData upsData;

  uint32_t runtimeSeconds;

  void handleClient(
    WiFiClient &client
  );

  void sendLine(
    WiFiClient &client,
    String text
  );

  void processCommand(
    WiFiClient &client,
    String cmd
  );

};

#endif
