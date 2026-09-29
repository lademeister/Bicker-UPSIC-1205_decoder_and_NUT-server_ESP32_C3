#include "runtime.h"



void RuntimeEstimator::begin()
{

  /*
     Standardwerte
  */

  V_full = 11.5;

  V_90 = 10.55;

  V_shutdown = 1.6;


  energyRate = 0;



  batteryStartTime = 0;

  t90 = 0;



  running = false;

  calibrated90 = false;
  
  seenAbove90 = false;

  lastState = 0;


  runtimeSeconds = 0;



  loadCalibration();

}







void RuntimeEstimator::loadCalibration()
{

  prefs.begin(
    "runtime",
    true
  );


  bool valid =
    prefs.getBool(
      "valid",
      false
    );


  if(valid)
  {

    V_full =
      prefs.getFloat(
        "vfull",
        V_full
      );


    V_90 =
      prefs.getFloat(
        "v90",
        V_90
      );


    energyRate =
      prefs.getFloat(
        "erate",
        0
      );


    calibrated90 =
      true;



    Serial.println(
      "Runtime calibration loaded"
    );


    Serial.print(
      "V_full="
    );

    Serial.println(V_full);



    Serial.print(
      "V_90="
    );

    Serial.println(V_90);



    Serial.print(
      "EnergyRate="
    );

    Serial.println(energyRate);

  }


  prefs.end();

}







void RuntimeEstimator::saveCalibration()
{

  prefs.begin(
    "runtime",
    false
  );


  prefs.putBool(
    "valid",
    true
  );


  prefs.putFloat(
    "vfull",
    V_full
  );


  prefs.putFloat(
    "v90",
    V_90
  );


  prefs.putFloat(
    "erate",
    energyRate
  );


  prefs.end();



  Serial.println(
    "Runtime calibration saved"
  );
Serial.println("Saving runtime calibration");

Serial.print("V_full=");
Serial.println(V_full);

Serial.print("V_90=");
Serial.println(V_90);

Serial.print("EnergyRate=");
Serial.println(energyRate);
}







//void RuntimeEstimator::update(
//  float voltage,
//  uint8_t state,
//  bool onBattery
//)
void RuntimeEstimator::update(
  float soc,
  float current,
  bool onBattery,
  uint8_t state,
  float voltage
)
{


  /*
     Voll geladen erkennen
  */

  if(state == 37)
  {

    V_full = voltage;

  }


  if(state == 42)
  {
    seenAbove90 = true;
  }
  /*
     Start Batteriebetrieb
  */

  if(
      onBattery &&
      !running
    )
  {

    batteryStartTime =
      millis();


    running = true;

  }





  /*
     Netzbetrieb
  */

  if(!onBattery)
  {

    running = false;

    runtimeSeconds = 0;


    lastState = state;


    return;

  }



  if(state != lastState)
  {
    Serial.print("Runtime state transition: ");
    Serial.print(lastState);
    Serial.print(" -> ");
    Serial.println(state);
  }

  /*
     90%-Punkt erkennen

     42 -> 10
  */

  if(
      lastState == 42 &&
      state == 10
    )
  {
    Serial.println("90% calibration point detected");
    V_90 = voltage;


    unsigned long elapsed =
      (
        millis() -
        batteryStartTime
      )
      /
      1000;



    t90 = elapsed;



    if(t90 > 0)
    {

      float eFull =
        V_full *
        V_full;


      float e90 =
        V_90 *
        V_90;



      energyRate =
        (
          eFull -
          e90
        )
        /
        t90;



      calibrated90 = true;


      saveCalibration();

    }

  }





  /*
     Runtime berechnen
  */

  if(
      calibrated90 &&
      energyRate > 0
    )
  {

    float eNow =
      voltage *
      voltage;



    float eMin =
      V_shutdown *
      V_shutdown;



    float remaining =
      eNow -
      eMin;



    if(remaining > 0)
    {

      runtimeSeconds =
        remaining /
        energyRate;

    }
    else
    {

      runtimeSeconds = 0;

    }

  }





  if(runtimeSeconds > 7200)
    runtimeSeconds = 7200;



  lastState = state;

}







uint32_t RuntimeEstimator::getRuntimeSeconds()
{
  return runtimeSeconds;
}







uint32_t RuntimeEstimator::getRuntimeLowSeconds()
{
  return 15;
}
