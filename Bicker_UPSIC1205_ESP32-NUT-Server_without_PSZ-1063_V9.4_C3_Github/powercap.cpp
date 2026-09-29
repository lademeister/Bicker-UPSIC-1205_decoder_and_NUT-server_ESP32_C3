//#include "powercap.h"
//
//
//void PowerCap::begin()
//{
//  Wire.begin();
//
//  data.state = 0;
//  data.voltage = 0;
//  data.current = 0;
//  data.soc = 0;
//}
//
//
//
//bool PowerCap::update()
//{
//
//  data.state = readRegister(REG_STATE) & 0xFF;
//
//  data.stateText = state1B(data.state);
//
//
//  data.raw25 = readRegister(REG_SOC_RAW);
//
//  data.raw26 = readRegister(REG_CAP_RAW);
//
//  data.raw27 = readRegister(REG_ESR1);
//
//  data.raw28 = readRegister(REG_ESR2);
//
//  data.raw29 = readSignedRegister(REG_CURRENT);
//
//  data.raw2A = readRegister(REG_MISC);
//
//
//
//  data.voltage = calculateVoltage(data.raw26);
//
//
//  data.current = ((float)data.raw29) * CURRENT_SCALE;
//
//
//  data.soc = calculateSOC(data.voltage);
//
//
//
//  // Zustände aus 1B
//
//  data.online = false;
//  data.charging = false;
//  data.onBattery = false;
//
//
//  switch(data.state)
//  {
//
//    case 37:
//      data.online = true;
//      break;
//
//
//    case 46:
//    case 42:
//    case 10:
//      data.onBattery = true;
//      break;
//
//
//    case 129:
//    case 145:
//    case 177:
//    case 49:
//    case 53:
//    case 17:
//    case 1:
//      data.online = true;
//      data.charging = true;
//      break;
//
//  }
//
//
//  data.lowBattery = false;
//
//
//  if(data.state == LOW_BAT_STATE)
//  {
//    data.lowBattery = true;
//  }
//
//
//  return true;
//
//}
//
//
//
//
//uint16_t PowerCap::readRegister(uint8_t reg)
//{
//
//  Wire.beginTransmission(POWERCAP_I2C_ADDR);
//
//  Wire.write(reg);
//
//  if(Wire.endTransmission(false)!=0)
//    return 0;
//
//
//  Wire.requestFrom(
//    POWERCAP_I2C_ADDR,
//    (uint8_t)2
//  );
//
//
//  if(Wire.available()<2)
//    return 0;
//
//
//  uint16_t value;
//
//  value  = Wire.read();
//  value |= ((uint16_t)Wire.read()<<8);
//
//
//  return value;
//}
//
//
//
//
//int16_t PowerCap::readSignedRegister(uint8_t reg)
//{
//
//  return (int16_t)readRegister(reg);
//
//}
//
//
//
//
//float PowerCap::calculateVoltage(uint16_t raw)
//{
//
//  /*
//    Ermittelt aus deinen Messpunkten:
//
//    RAW 7063 -> 11.579V
//    RAW 995  -> 1.632V
//
//    Faktor:
//    ca. 0.00164 V/count
//  */
//
//
//  return raw * CAP_VOLTAGE_SCALE;
//
//}
//
//
//
//
//float PowerCap::calculateSOC(float v)
//{
//
//  /*
//    Spannungsverlauf aus deinen Logs:
//
//    11.58V  = 100%
//    8.56V   = ~90%
//    5.7V    = ~50%
//    3.2V    = ~10%
//    1.6V    = 0%
//
//    lineare Näherung zwischen Punkten
//  */
//
//
//  if(v >= 11.5)
//    return 100;
//
//
//  if(v >= 8.5)
//    return 90 + (v-8.5)*3.3;
//
//
//  if(v >= 5.7)
//    return 50 + (v-5.7)*14.3;
//
//
//  if(v >= 3.2)
//    return 10 + (v-3.2)*14.3;
//
//
//  if(v >= 1.6)
//    return (v-1.6)*6.25;
//
//
//  return 0;
//
//}
//
//
//
//
//const char* PowerCap::state1B(uint8_t v)
//{
//
//  switch(v)
//  {
//
//    case 37:
//      return "FULL / GRID";
//
//
//    case 46:
//      return "GRID->BAT transition";
//
//
//    case 42:
//      return "BAT >90%";
//
//
//    case 10:
//      return "BAT <90%";
//
//
//    case 129:
//      return "GRID RETURN";
//
//
//    case 145:
//      return "CHARGING <90%";
//
//
//    case 177:
//      return "CHARGING >90%";
//
//
//    case 49:
//    case 53:
//    case 17:
//    case 1:
//      return "CHARGING transition";
//
//
//    default:
//      return "UNKNOWN";
//
//  }
//
//}
//
//
//
//PowerCapData PowerCap::getData()
//{
//  return data;
//}

#include "powercap.h"



void PowerCap::begin()
{

  Wire.begin(
    I2C_SDA,
    I2C_SCL
  );


  memset(
    &data,
    0,
    sizeof(data)
  );
  Wire.setClock(100000);
  Wire.setTimeOut(50);
}

//float PowerCap::calculateSOC(float voltage)
//{
//
//  if(voltage >= 11.5)
//    return 100;
//
//
//  if(voltage >= 11.2)
//    return 95 +
//      (voltage-11.2) *
//      (5.0/0.3);
//
//
//  if(voltage >= 10.8)
//    return 85 +
//      (voltage-10.8) *
//      (10.0/0.4);
//
//
//  if(voltage >= 10.3)
//    return 70 +
//      (voltage-10.3) *
//      (15.0/0.5);
//
//
//  if(voltage >= 9.8)
//    return 55 +
//      (voltage-9.8) *
//      (15.0/0.5);
//
//
//  if(voltage >= 9.3)
//    return 40 +
//      (voltage-9.3) *
//      (15.0/0.5);
//
//
//  if(voltage >= 8.8)
//    return 25 +
//      (voltage-8.8) *
//      (15.0/0.5);
//
//
//  if(voltage >= 8.0)
//    return 10 +
//      (voltage-8.0) *
//      (15.0/0.8);
//
//
//  if(voltage >= 1.6)
//    return (voltage-1.6) *
//      (10.0/6.4);
//
//
//  return 0;
//
//}
//

bool PowerCap::update()
{

  data.state =
    read16(REG_STATE) & 0xFF;


  data.stateText =
    state1B(data.state);



  data.raw25 =
    read16(REG_SOC_RAW);


  data.raw26 =
    read16(REG_CAP_RAW);


  data.raw27 =
    read16(REG_ESR1);


  data.raw28 =
    read16(REG_ESR2);


  data.raw29 =
    readSigned16(REG_CURRENT);


  data.raw2A =
    read16(REG_MISC);



  data.voltage =
    calculateVoltage(
      data.raw26
    );



  data.current =
    (float)data.raw29 *
    CURRENT_FACTOR;



  /*
     Leistung:
     
     Strom ist signed:
     
     negativ = Entladung
     positiv = Laden

     Für NUT brauchen wir
     den Verbrauch positiv
  */


  data.power =
    fabs(
      data.voltage *
      data.current /
      1000.0
    );



//  data.soc =
//    calculateSOC(
//      data.voltage
//    );

    data.soc =
      calculateSOC(
        data.voltage,
        data.state
    );


  data.online=false;
  data.charging=false;
  data.onBattery=false;



  switch(data.state)
  {

    case 37:
      data.online=true;
      break;



    case 46:
    case 42:
    case 10:
      data.onBattery=true;
      break;



    case 129:
    case 145:
    case 177:
    case 49:
    case 53:
    case 17:
    case 1:

      data.online=true;
      data.charging=true;
      break;

  }



  data.lowBattery =
    (
      data.state == 10
    );


  return true;

}




uint16_t PowerCap::read16(
  uint8_t reg
)
{

  Wire.beginTransmission(
    I2C_ADDRESS
  );


  Wire.write(reg);


  if(
    Wire.endTransmission(false)
    !=0
  )
  {
    return 0;
  }



  Wire.requestFrom(
    I2C_ADDRESS,
    (uint8_t)2
  );



  if(
    Wire.available()!=2
  )
  {
    return 0;
  }



  uint16_t value;


  value =
    Wire.read();


  value |=
    ((uint16_t)Wire.read()<<8);



  return value;

}




int16_t PowerCap::readSigned16(
  uint8_t reg
)
{

  return (int16_t)
    read16(reg);

}





float PowerCap::calculateVoltage(
  uint16_t raw
)
{

  return
    raw *
    CAP_VOLT_FACTOR;

}




//
//float PowerCap::calculateSOC(
//  float v
//)
//{
//
//  /*
//     Tabelle aus realer Messung
//  */
//
//
//  if(v >= 8.57)
//    return 90;
//
//
//  if(v >= 7.5)
//    return map(
//      v*100,
//      750,
//      857,
//      80,
//      90
//    );
//
//
//  if(v >= 6.5)
//    return map(
//      v*100,
//      650,
//      750,
//      65,
//      80
//    );
//
//
//  if(v >= 5.7)
//    return map(
//      v*100,
//      570,
//      650,
//      50,
//      65
//    );
//
//
//  if(v >= 4.8)
//    return map(
//      v*100,
//      480,
//      570,
//      35,
//      50
//    );
//
//
//  if(v >= 3.2)
//    return map(
//      v*100,
//      320,
//      480,
//      15,
//      35
//    );
//
//
//  if(v >= 2.0)
//    return map(
//      v*100,
//      200,
//      320,
//      5,
//      15
//    );
//
//
//  if(v >= 1.63)
//    return map(
//      v*100,
//      163,
//      200,
//      0,
//      5
//    );
//
//
//  return 0;
//
//}
//
//


float PowerCap::calculateSOC(
  float voltage,
  uint8_t state
)
{

  static float V_full = 11.5;     // wird bei FULL aktualisiert
  const float V_90 = 10.55;
  const float V_min = 1.6;


  // ---------------------------------
  // Voll geladen
  // ---------------------------------

  if(state == 37)
  {
    V_full = voltage;
    return 100.0;
  }



  // ---------------------------------
  // Bereich 90-100%
  // ---------------------------------

  if(voltage >= V_90)
  {

    float soc =
      90.0 +
      (
        (voltage - V_90)
        /
        (V_full - V_90)
      )
      * 10.0;


    if(soc > 100)
      soc = 100;


    return soc;

  }



  // ---------------------------------
  // Bereich <90%
  // Energieinhalt Supercap
  // ---------------------------------

  float soc =
    90.0 *
    (
      (voltage * voltage)
      -
      (V_min * V_min)
    )
    /
    (
      (V_90 * V_90)
      -
      (V_min * V_min)
    );


  if(soc < 0)
    soc = 0;


  return soc;

}


//const char* PowerCap::state1B(
//  uint8_t s
//)
//{
//
//  switch(s)
//  {
//
//    case 37:
//      return "FULL / GRID";
//
//
//    case 46:
//      return "GRID->BAT transition";
//
//
//    case 42:
//      return "BAT >90%";
//
//
//    case 10:
//      return "BAT <90%";
//
//
//    case 129:
//      return "GRID RETURN";
//
//
//    case 145:
//      return "CHARGING <90%";
//
//
//    case 177:
//      return "CHARGING >90%";
//
//
//    case 49:
//    case 53:
//    case 17:
//    case 1:
//      return "CHARGING transition";
//
//
//    default:
//      return "UNKNOWN";
//
//  }
//
//}

const char* PowerCap::state1B(
  uint8_t s
)
{
  // Statischer Puffer bietet Platz für bis zu 3 Ziffern + Nullterminator
  static char buffer[20]; 
  
  switch(s)
  {

    case 37:
//      return "FULL /GRID";
//      return "INPUT BACK";
      return "AM NETZ";
    case 46:
      return "NETZ->BAT";
    case 42:
//      return "BAT >90%";
      return "NETZFEHLER";
//      return "CHARGE>90%";
    case 10:
      //return "BAT <90%";
//      return "CHARGE<90%";
      return "NOTSTROM";

    case 129:
//    return "GRID BACK";
//    return "INPUT BACK";
      return "NETZ OK";
    case 145:
//      return "CHG <90%";
      return "LADEN <90%";
    case 165:
      return "GELADEN";
    case 181:
      return "VOLL";
    case 177:
//      return "CHG >90%";
      return "LADEN >90%";
    case 49:
    case 53:
    case 17:
    case 1:
      return "CHG trans";


    default:
      return "";
      //return "UNKNOWN";

//// Wandelt die Zahl s (uint8_t) in eine Zeichenkette um und schreibt sie in den Puffer
//      itoa(s, buffer, 10); 
//      return buffer; // Gibt den Zeiger auf den Puffer zurück

  }

}


PowerCapData PowerCap::getData()
{
  return data;
}
