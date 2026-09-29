#ifndef RUNTIME_H
#define RUNTIME_H

#include <Arduino.h>
#include <Preferences.h>


class RuntimeEstimator
{

public:

  void begin();


//  void update(
//    float voltage,
//    uint8_t state,
//    bool onBattery
//  );

  void update(
    float soc,
    float current,
    bool onBattery,
    uint8_t state,
    float voltage
  );


  uint32_t getRuntimeSeconds();

  uint32_t getRuntimeLowSeconds();



private:

  Preferences prefs;


  // -------------------------
  // Kalibrierwerte
  // -------------------------

  float V_full;

  float V_90;

  float V_shutdown;


  float energyRate;



  // -------------------------
  // Laufzeit
  // -------------------------

  unsigned long batteryStartTime;

  unsigned long t90;



  // -------------------------
  // Zustände
  // -------------------------

  bool running;

  bool calibrated90;

  bool seenAbove90;


  uint8_t lastState;



  uint32_t runtimeSeconds;



  // -------------------------
  // EEPROM / NVS
  // -------------------------

  void loadCalibration();

  void saveCalibration();


};


#endif
