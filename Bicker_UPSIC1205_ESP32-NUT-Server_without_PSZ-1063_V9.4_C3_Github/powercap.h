//#ifndef POWERCAP_H
//#define POWERCAP_H
//
//#include <Arduino.h>
//#include <Wire.h>
//#include "config.h"
//
//
//struct PowerCapData
//{
//  uint8_t state;
//
//  const char* stateText;
//
//  uint16_t raw25;
//  uint16_t raw26;
//  uint16_t raw27;
//  uint16_t raw28;
//  int16_t  raw29;
//  uint16_t raw2A;
//
//  float voltage;
//  float current;
//
//  float soc;
//
//  bool online;
//  bool charging;
//  bool onBattery;
//  bool lowBattery;
//};
//
//
//
//class PowerCap
//{
//
//public:
//
//  void begin();
//
//  bool update();
//
//  PowerCapData getData();
//
//
//private:
//
//  PowerCapData data;
//
//
//  uint16_t readRegister(uint8_t reg);
//
//  int16_t readSignedRegister(uint8_t reg);
//
//
//  const char* state1B(uint8_t v);
//
//
//  float calculateVoltage(uint16_t raw);
//
//  float calculateSOC(float voltage);
//
//};
//
//
//#endif



#ifndef POWERCAP_H
#define POWERCAP_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"


struct PowerCapData
{
  uint8_t state;
  const char* stateText;


  uint16_t raw25;
  uint16_t raw26;
  uint16_t raw27;
  uint16_t raw28;
  int16_t  raw29;
  uint16_t raw2A;


  float voltage;
  float current;
  float power;


  float soc;


  bool online;
  bool charging;
  bool onBattery;
  bool lowBattery;
};



class PowerCap
{

public:

  void begin();


  bool update();


  PowerCapData getData();



private:

  PowerCapData data;


  uint16_t read16(uint8_t reg);

  int16_t readSigned16(uint8_t reg);



  float calculateVoltage(
    uint16_t raw
  );


//  float calculateSOC(
//    float voltage
//  );

float calculateSOC(
  float voltage,
  uint8_t state
);


  const char* state1B(
    uint8_t state
  );


};



#endif
