#include "nutserver.h"



void NUTServer::begin()
{

  server.begin();

}



void NUTServer::update(
  PowerCapData data,
  RuntimeEstimator runtime
)
{

  upsData = data;

  runtimeSeconds =
      runtime.getRuntimeSeconds();



//  WiFiClient client =
//      server.available();
//
//
//  if(client)
//  {
//
//    handleClient(client);
//
//  }

}


void NUTServer::handle()
{

  WiFiClient client =
      server.available();


  if(client)
  {
    handleClient(client);
  }

}

//void NUTServer::handleClient(
//  WiFiClient &client
//)
//{
//
//  client.setTimeout(2000);
//
//
//  while(client.connected())
//  {
//
//    if(client.available())
//    {
//
//      String cmd =
//        client.readStringUntil('\n');
//
//
//      cmd.trim();
//      Serial.print("NUT CMD: ");
//      Serial.println(cmd);
//
//      processCommand(
//        client,
//        cmd
//      );
//
//
//    }
//
//
//    delay(1);
//
//  }
//
//
//  client.stop();
//
//}


//void NUTServer::handleClient(
//  WiFiClient &client
//)
//{
//
//  client.setTimeout(2000);
//
//
//  while(client.connected())
//  {
//
//    if(client.available())
//    {
//
//      String cmd =
//        client.readStringUntil('\n');
//
//
//      cmd.trim();
//
//      Serial.print("NUT CMD: ");
//      Serial.println(cmd);
//
//
//      processCommand(
//        client,
//        cmd
//      );
//
//
//      // upsc öffnet für jeden Request
//      // eine neue Verbindung.
//      // Danach sofort schließen, damit
//      // der ESP nicht hier hängen bleibt.
//
//      break;
//
//    }
//
//
//    delay(1);
//
//  }
//
//
//  client.stop();
//
//}
//void NUTServer::handleClient(
//  WiFiClient &client
//)
//{
//
//  client.setTimeout(2000);
//
//
//  while(client.connected())
//  {
//
//    if(client.available())
//    {
//
//      String cmd =
//        client.readStringUntil('\n');
//
//
//      cmd.trim();
//
//      Serial.print("NUT CMD: ");
//      Serial.println(cmd);
//
//
//      processCommand(
//        client,
//        cmd
//      );
//
//    }
//
//
//    delay(1);
//
//  }
//
//
//  client.stop();
//
//}

//void NUTServer::handleClient(
//  WiFiClient &client
//)
//{
//
//  client.setTimeout(2000);
//
//  unsigned long lastActivity = millis();
//
//
//  while(
//    client.connected() &&
//    millis() - lastActivity < 5000
//  )
//  {
//
//    if(client.available())
//    {
//
//      String cmd =
//        client.readStringUntil('\n');
//
//
//      lastActivity = millis();
//
//
//      cmd.trim();
//
//
//      Serial.print("NUT CMD: ");
//      Serial.println(cmd);
//
//
//      processCommand(
//        client,
//        cmd
//      );
//
//    }
//
//
//    delay(1);
//
//  }
//
//
//  client.stop();
//
//}

void NUTServer::handleClient(WiFiClient &client)
{
    if(!client.available())
        return;


    String cmd =
        client.readStringUntil('\n');


    cmd.trim();


    Serial.print("NUT CMD: ");
    Serial.println(cmd);


    processCommand(
        client,
        cmd
    );


    client.stop();
}

void NUTServer::sendLine(
  WiFiClient &client,
  String text
)
{

  client.print(text);
  client.print("\n");

}




void NUTServer::processCommand(
  WiFiClient &client,
  String cmd
)
{

  if(cmd=="STARTTLS")
  {
    sendLine(
      client,
      "ERR FEATURE-NOT-CONFIGURED"
    );

    return;
  }
// ----------------------------
// VERSION
// ----------------------------


if(cmd=="VER")
{

  sendLine(
    client,
    "Network UPS Tools ESP32"
  );

  return;

}



// ----------------------------
// NET VERSION
// ----------------------------


if(cmd=="NETVER")
{

  sendLine(
    client,
    "2.0"
  );

  return;

}



// ----------------------------
// LIST UPS
// ----------------------------


if(cmd=="LIST UPS")
{

  sendLine(
    client,
    "BEGIN LIST UPS"
  );


  sendLine(
    client,
    "UPS " +
    String(UPS_NAME) +
    " \"" +
    String(UPS_MODEL) +
    "\""
  );


  sendLine(
    client,
    "END LIST UPS"
  );


  return;

}



// ----------------------------
// UPSDESC
// ----------------------------


if(cmd.startsWith("GET UPSDESC"))
{

  sendLine(
    client,
    "UPSDESC " +
    String(UPS_NAME) +
    " \"" +
    String(UPS_MODEL) +
    "\""
  );


  return;

}



// ----------------------------
// LIST VARIABLES
// ----------------------------


if(cmd.startsWith("LIST VAR"))
{

  sendLine(
    client,
    "BEGIN LIST VAR " +
    String(UPS_NAME)
  );


  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " device.mfr \"" +
  UPS_VENDOR + "\"");


  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " device.model \"" +
  UPS_MODEL + "\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " battery.charge \"" +
  String((int)upsData.soc) + "\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " battery.voltage \"" +
  String(upsData.voltage,2) + "\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " battery.current \"" +
  String(upsData.current/1000.0,2) + "\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " battery.runtime \"" +
  String(runtimeSeconds) + "\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " battery.runtime.low \"40\"");



  sendLine(client,
  "VAR " + String(UPS_NAME) +
  " ups.status \"" +
  getStatus() + "\"");



  sendLine(client,
  "END LIST VAR " +
  String(UPS_NAME));


  return;

}




// ----------------------------
// GET VAR - answers with different entities to an upsmon request
// ----------------------------


//if(cmd.startsWith("GET VAR"))
//{
//
//  String var =
//    cmd.substring(
//      cmd.lastIndexOf(' ')+1
//    );
//
//
//
//  String value="0";
//
//
//
//  if(var=="battery.charge")
//    value=String((int)upsData.soc);
//
//
//
//  else if(var=="battery.voltage")
//    value=String(upsData.voltage,2);
//
//
//
//  else if(var=="battery.current")
//    value=String(upsData.current/1000.0,2);
//
//
//
//  else if(var=="battery.runtime")
//    value=String(runtimeSeconds);
//
//
//
//  else if(var=="ups.status")
//    value=getStatus();
//
//
//
//  sendLine(
//    client,
//    "VAR " +
//    String(UPS_NAME) +
//    " " +
//    var +
//    " \"" +
//    value +
//    "\""
//  );
//
//
//  return;
//
//}
if(cmd.startsWith("GET VAR"))
{

  String var =
    cmd.substring(
      cmd.lastIndexOf(' ')+1
    );


  String value="0";


  if(var=="battery.charge")
    value=String((int)upsData.soc);


  else if(var=="battery.voltage")
    value=String(upsData.voltage,2);


  else if(var=="battery.current")
    value=String(upsData.current/1000.0,2);


  else if(var=="battery.runtime")
    value=String(runtimeSeconds);


  else if(var=="battery.runtime.low")
    value="40";


  else if(var=="device.mfr")
    value=String(UPS_VENDOR);


  else if(var=="device.model")
    value=String(UPS_MODEL);


  else if(var=="ups.status")
    value=getStatus();


  sendLine(
    client,
    "VAR " +
    String(UPS_NAME) +
    " " +
    var +
    " \"" +
    value +
    "\""
  );


  return;

}

}





String NUTServer::getStatus()
{

  if(upsData.lowBattery)
    return "OB LB";


  if(upsData.onBattery)
    return "OB DISCHRG";


  if(upsData.charging)
    return "OL CHRG";


  return "OL";

}
