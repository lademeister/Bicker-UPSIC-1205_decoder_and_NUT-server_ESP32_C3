#include <Arduino.h>

#include "config.h"
#include "powercap.h"
#include "nutserver.h"
#include "runtime.h"
#include "ota.h"
#include "wifi_setup.h"
#include "ha_discovery.h"
#include <Wire.h>
//#include "SSD1306Ascii.h"
//#include "SSD1306AsciiWire.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>


HADiscovery haDiscovery;

WiFiClient mqttWiFiClient;
PubSubClient mqttClient(mqttWiFiClient);
unsigned long lastMQTTReconnect = 0;


uint8_t ledBrightness = 25;
Adafruit_NeoPixel ledstrip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RGB_LED_onboard(1, 8, NEO_GRB + NEO_KHZ800);

//SSD1306AsciiWire oled;


static bool lastOnBattery = false;
static bool powerLossFlash = false;
static uint8_t flashCount = 0;
static bool flashState = false;
static unsigned long lastFlash = 0;


PowerCap powercap;

NUTServer nut;

RuntimeEstimator runtime;

OTAUpdater ota;

WiFiManagerESP wifi;

unsigned long lastPowerUpdate = 0;
unsigned long lastOLEDUpdate = 0;
unsigned long lastLEDUpdate = 0;
uint32_t lastMotionTime = 0;
bool oledOn = false;

PowerCapData displayData;

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

#include <Fonts/FreeSans9pt7b.h>
/*
FreeSans9pt7b
FreeSans12pt7b
FreeSansBold12pt7b
FreeMono9pt7b
FreeMonoBold12pt7b
 */


unsigned long lastUpdate = 0;

//void init_oled(){
//  //Wire.begin();         
//  oled.begin(&Adafruit128x32, I2C_ADDRESS_OLED);
//  oled.set400kHz();  
//  oled.setFont(Adafruit5x7); 
//  oled.set2X(); 
//
//  uint32_t m = micros();
//  oled.clear();  
//  oled.println("Bicker");
//  oled.println("UPSIC-1205");
//  delay(1000);
//  oled.clear();  
//}


void init_oled(){
  if(!display.begin(SSD1306_SWITCHCAPVCC,I2C_ADDRESS_OLED))
  {
    Serial.println("##### OLED failed #####");
    //while(true);
  }
  else{
    display.ssd1306_command(SSD1306_SEGREMAP | 0x0);
    display.ssd1306_command(SSD1306_COMSCANINC);


    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Bicker");
    display.println("UPSIC-1205");
    display.display();
    delay(2500);
    display.clearDisplay();
    display.display();
  }
}

void init_neopixel(){
  ledstrip.begin();
  RGB_LED_onboard.begin();
  ledstrip.setBrightness(LEDstrip_global_brightness_setting);
  RGB_LED_onboard.setBrightness(50);
//  RGB_LED_onboard.setPixelColor(0, 0, 0, 255);
//  delay(100);
//  RGB_LED_onboard.setPixelColor(0, 0, 255, 0);
//  delay(100);
  RGB_LED_onboard.setPixelColor(0, 0, 0, 0);
  //for (int i = 0; i < NUM_LEDS; i++) ledstrip.setPixelColor(i, rot, gruen, blau);
  for (int i = 0; i < NUM_LEDS; i++) ledstrip.setPixelColor(i, 0, 25, 0);
  delay(100);
  ledstrip.show();
  for (int i = 0; i < NUM_LEDS; i++) ledstrip.setPixelColor(i, 0, 0, 0);
  //ledstrip.setPixelColor(2, 5, 0, 0);
  ledstrip.show();
}

uint32_t fadeColor(
  uint32_t oldColor,
  uint32_t newColor,
  float amount
)
{

  uint8_t r1 =
    (oldColor >> 16) & 0xFF;

  uint8_t g1 =
    (oldColor >> 8) & 0xFF;

  uint8_t b1 =
    oldColor & 0xFF;


  uint8_t r2 =
    (newColor >> 16) & 0xFF;

  uint8_t g2 =
    (newColor >> 8) & 0xFF;

  uint8_t b2 =
    newColor & 0xFF;



  uint8_t r =
    r1 + (r2-r1)*amount;

  uint8_t g =
    g1 + (g2-g1)*amount;

  uint8_t b =
    b1 + (b2-b1)*amount;


  return ledstrip.Color(r,g,b);

}

//void updateLEDs()
//{
//
//  static uint32_t currentColors[NUM_LEDS];
//
//  static bool blinkState = false;
//
//  static unsigned long lastBlink = 0;
//
//
//  if(
//    millis()-lastBlink > 500
//  )
//  {
//
//    lastBlink = millis();
//
//    blinkState = !blinkState;
//
//  }
//
//
//
//  float soc =
//    constrain(
//      displayData.soc,
//      0,
//      100
//    );
//
//
//  uint8_t activeLEDs =
//    map(
//      soc,
//      0,
//      100,
//      0,
//      NUM_LEDS
//    );
//
//
//
//  bool critical =
//    (
//      displayData.onBattery &&
//      soc < 90
//    );
//
//
//
//  bool charging =
//    displayData.charging;
//
//
//
//  for(
//    int i=0;
//    i<NUM_LEDS;
//    i++
//  )
//  {
//    int pixel = NUM_LEDS - 1 - i;
//
//    uint32_t target;
//
//
//
//    // -------------------------
//    // Charging animation
//    // -------------------------
//
//    if(charging)
//    {
//
//      uint8_t wave =
//        //(millis()/50+i*20)%255;
//        (millis()/50+(NUM_LEDS-1-i)*20)%255; //inverted direction
//
//
//      target =
//        ledstrip.Color(
//          wave,
//          0,
//          80
//        );
//
//    }
//
//
//
//
//
//
//
//    // -------------------------
//    // SOC bargraph
//    // -------------------------
//
//    else if(i < activeLEDs)
//    {
//
//      if(soc > 60)
//      {
//
//        target =
//          ledstrip.Color(
//            0,
//            ledBrightness,
//            0
//          );
//
//      }
//
//      else if(soc > 30)
//      {
//
//        target =
//          ledstrip.Color(
//            ledBrightness,
//            ledBrightness,
//            0
//          );
//
//      }
//
//      else
//      {
//
//        target =
//          ledstrip.Color(
//            ledBrightness,
//            0,
//            0
//          );
//
//      }
//
//    }
//
//
//    // -------------------------
//    // empty LEDs
//    // -------------------------
//
//    else
//    {
//
//      target =
//        ledstrip.Color(8,0,0);
//    }
//
//
//
//    // -------------------------
//    // first LED warning blink
//    // -------------------------
//
//    if(
//      critical &&
//      i==0
//    )
//    {
//
//      if(blinkState)
//      {
//        target =
//          ledstrip.Color(
//            0,
//            ledBrightness,
//            0
//          );
//      }
//      else
//      {
//        target =
//          ledstrip.Color(
//            ledBrightness,
//            0,
//            0
//          );
//      }
//
//    }
//
//
//
//    uint32_t old =
//      currentColors[i];
//
//
//    currentColors[i] =
//      fadeColor(
//        old,
//        target,
//        0.15
//      );
//
//
//    //ledstrip.setPixelColor(i,currentColors[i]);
//    ledstrip.setPixelColor(pixel,currentColors[i]);
//
//
//  }
//
//
//  ledstrip.show();
//
//}


//void updateLEDs()
//{
//  static uint32_t currentColors[NUM_LEDS];
//  static bool blinkState = false;
//  static bool fastBlinkState = false;
//  static unsigned long lastBlink = 0;
//  static unsigned long lastFastBlink = 0;
//
//  if(millis()-lastBlink > 500)
//  {
//    lastBlink = millis();
//    blinkState = !blinkState;
//  }
//
//  if(millis()-lastFastBlink > 150)
//  {
//    lastFastBlink = millis();
//    fastBlinkState = !fastBlinkState;
//  }
//
//
//  float soc = constrain(displayData.soc,0,100);
//
//  uint8_t activeLEDs = map(soc,0,100,0,NUM_LEDS);
//
//  bool critical = displayData.onBattery && soc < 90;
//  bool veryLow = displayData.onBattery && soc < 30;
//  bool charging = displayData.charging;
//
//
//  uint32_t barColor;
//
//  if(soc > 60)
//    barColor = ledstrip.Color(0,ledBrightness,0);
//  else if(soc > 30)
//    barColor = ledstrip.Color(ledBrightness,ledBrightness,0);
//  else
//    barColor = ledstrip.Color(ledBrightness,0,0);
//
//
//
//  for(int i=0;i<NUM_LEDS;i++)
//  {
//    int pixel = NUM_LEDS-1-i;
//
//    uint32_t target;
//
//
//    // SOC bargraph
//    if(i < activeLEDs)
//    {
//      target = barColor;
//    }
//
//    // inactive LEDs
//    else
//    {
//      if(charging)
//        target = ledstrip.Color(8,0,8);   // dark purple
//      else
//        target = ledstrip.Color(8,0,0);   // dark red
//    }
//
//
//
//    // low battery warning
//    if(critical && i==0)
//    {
//
//      if(veryLow)
//      {
//        // fast red flash below 30%
//        if(fastBlinkState)
//          target = ledstrip.Color(ledBrightness,0,0);
//        else
//          target = ledstrip.Color(0,0,0);
//      }
//      else
//      {
//        // normal warning blink
//        if(blinkState)
//          target = barColor;
//        else
//          target = ledstrip.Color(ledBrightness,0,0);
//      }
//
//    }
//
//
//
//    currentColors[i] =
//      fadeColor(
//        currentColors[i],
//        target,
//        0.15
//      );
//
//
//    ledstrip.setPixelColor(
//      pixel,
//      currentColors[i]
//    );
//
//  }
//
//
//  ledstrip.show();
//}



void updateLEDs()
{
//  static uint32_t currentColors[NUM_LEDS];
//  static bool blinkState = false;
//  static bool fastBlinkState = false;
//  static unsigned long lastBlink = 0;
//  static unsigned long lastFastBlink = 0;

  static uint32_t currentColors[NUM_LEDS];
  static bool blinkState = false;
  static bool fastBlinkState = false;
  static unsigned long lastBlink = 0;
  static unsigned long lastFastBlink = 0;

  static bool lastOnBattery = false;
  static bool powerLossFlash = false;
  static uint8_t flashCount = 0;
  static bool flashState = false;
  static unsigned long lastFlash = 0;

  uint32_t now = millis();
  // detect transition from grid power to battery
if(displayData.onBattery && !lastOnBattery)
{
  powerLossFlash = true;
  flashCount = 0;
  flashState = true;
  lastFlash = now;
}

lastOnBattery = displayData.onBattery;


// 4x full red flash on power loss
  if(powerLossFlash)
  {
    if(now - lastFlash > 200)
    {
      lastFlash = now;
      flashState = !flashState;
  
      if(!flashState)
      {
        flashCount++;
  
        if(flashCount >= 4)
          powerLossFlash = false;
      }
    }
  
  
    uint32_t flashColor;
  
    if(flashState){
      //flashColor = ledstrip.Color(ledBrightness,0,0);
      ledstrip.setBrightness(flashBrightness);
      flashColor = ledstrip.Color(flashBrightness,0,0);
    }
    else{
      flashColor = ledstrip.Color(0,0,0);
    }
  
    for(int i=0;i<NUM_LEDS;i++)
    {
      ledstrip.setPixelColor(i, flashColor);
    }
  
    ledstrip.show();
    return;
  }

  else{
    ledstrip.setBrightness(LEDstrip_global_brightness_setting);
  }

  
  if(millis()-lastBlink > 500)
  {
    lastBlink = millis();
    blinkState = !blinkState;
  }

  if(millis()-lastFastBlink > 150)
  {
    lastFastBlink = millis();
    fastBlinkState = !fastBlinkState;
  }


  float soc = constrain(displayData.soc,0,100);

  uint8_t activeLEDs = map(soc,0,100,0,NUM_LEDS);

  bool critical = displayData.onBattery && soc < 90;
  bool veryLow = displayData.onBattery && soc < 30;
  bool charging = displayData.charging;


  uint32_t barColor;

//  if(soc > 60)
//    barColor = ledstrip.Color(0,ledBrightness,0);
//  else if(soc > 30)
//    barColor = ledstrip.Color(ledBrightness,ledBrightness,0);
//  else
//    barColor = ledstrip.Color(ledBrightness,0,0);


    if(displayData.onBattery && soc < 30)
    {
      // critical battery
      barColor = ledstrip.Color(ledBrightness,0,0);
    }
    else if(displayData.onBattery && soc < 95)
    {
      // battery discharge warning (orange/red)
      barColor = ledstrip.Color(ledBrightness,60,0);
    }
    else
    {
      if(soc > 60)
        barColor = ledstrip.Color(0,ledBrightness,0);
      else if(soc > 30)
        barColor = ledstrip.Color(ledBrightness,ledBrightness,0);
      else
        barColor = ledstrip.Color(ledBrightness,0,0);
    }

  for(int i=0;i<NUM_LEDS;i++)
  {
    int pixel = NUM_LEDS-1-i;

    uint32_t target;


    // SOC bargraph
    if(i < activeLEDs)
    {
      target = barColor;
    }

    // inactive LEDs
    else
    {
      if(charging)
        target = ledstrip.Color(8,0,8);   // dark purple
      else
        target = ledstrip.Color(8,0,0);   // dark red
    }



//    // low battery warning
//    if(critical && i==0)
//    {
//
//      if(veryLow)
//      {
//        // fast red flash below 30%
//        if(fastBlinkState)
//          target = ledstrip.Color(ledBrightness,0,0);
//        else
//          target = ledstrip.Color(0,0,0);
//      }
//      else
//      {
//        // normal warning blink
//        if(blinkState)
//          target = barColor;
//        else
//          target = ledstrip.Color(ledBrightness,0,0);
//      }
//
//    }



    currentColors[i] =
      fadeColor(
        currentColors[i],
        target,
        0.15
      );


    ledstrip.setPixelColor(
      pixel,
      currentColors[i]
    );

  }


  ledstrip.show();
}

void setup()
{

  Serial.begin(115200);

  delay(500);



  Serial.println();
  Serial.println("===============================");
  Serial.println(" Bicker UPSIC-1205 NUT ESP32");
  Serial.println("===============================");

  pinMode(PIR_pin, INPUT_PULLDOWN);


  // -------------------------
  // I2C PowerCap
  // -------------------------

  powercap.begin(); //incl i2c init
  Serial.println("PowerCap initialized");


  init_oled();
  init_neopixel();

  runtime.begin();

  // -------------------------
  // WiFi
  // -------------------------

  wifi.begin();


  Serial.print("IP: ");
  Serial.println(
    wifi.getIP()
  );



  // -------------------------
  // OTA
  // -------------------------

  ota.begin();



  // -------------------------
  // NUT Server
  // -------------------------

  nut.begin();


  Serial.println(
    "NUT server started on port 3493"
  );

  mqttClient.setServer(
  MQTT_SERVER,
  MQTT_PORT
);
mqttClient.setBufferSize(1024);

haDiscovery.begin(mqttClient);

}


//void oled_update(){
//  oled.clear();  
//  oled.println("USV");
//  oled.print("SOC = ");
//  oled.print(data.soc,1);
//  oled.println(" %");
//}

void updateOLED()
{
//  if(displayData.soc>99){
//    display.clearDisplay();
//    display.display();
//    return;
//  }
//  static float lastVoltage = -1;
//  static float lastSOC = -1;
//  static uint8_t lastState = 255;

  static float lastVoltage = -1;
  static float lastSOC = -1;
  static uint8_t lastState = 255;
  static bool lastOnBattery = false;
  static bool powerLossFlash = false;


  if(
    displayData.soc == lastSOC &&
    displayData.soc>99
  )
  {
    return;
  }

  if(
    displayData.voltage == lastVoltage &&
    displayData.soc == lastSOC &&
    displayData.state == lastState
  )
  {
    return;
  }


  lastVoltage = displayData.voltage;
  lastSOC = displayData.soc;
  lastState = displayData.state;


  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
//
//  display.print("UPSIC1205");
//
//
//  display.setCursor(0,12);
  display.setTextSize(2);
  //display.setFont(&FreeSans9pt7b);
  display.setCursor(0, 3);
  display.print("SOC: ");
  display.print(displayData.soc,0);
  display.println("%");

//  display.setTextSize(2);
//  display.print("V: ");
//  display.print(displayData.voltage,2);
//  display.print("V - ");


  //display.println(displayData.stateText);
  if (displayData.state == 10)
  {
      display.print("Puffer:");
      display.print(runtime.getRuntimeSeconds());
      display.println("s");
  }
  else
  {
      display.println(displayData.stateText);
  }
  
  //display.println("-test-");


  display.display();

}



void display_off(){
    
    display.clearDisplay();
    display.display();
}



void handleOLEDTimeout()
{
    static bool lastPirState = false;

    bool pirState = digitalRead(PIR_pin) == HIGH;
  
    
    
//  //debug_pir_state:
//      if (pirState!=lastPirState){
//      if (pirState){    
//        ledstrip.setPixelColor(NUM_LEDS-11, 255, 0, 0);
//        ledstrip.show();
//      }
//      else{
//        ledstrip.setPixelColor(NUM_LEDS-11, 0, 0, 255);
//        ledstrip.show();
//      }
//    }
//
//  //END of debug pir state  


    
    // Motion detected
    if (pirState)
    {
        lastMotionTime = millis();

        if (!oledOn)
        {
            oledOn = true;
//            display_on();aaaa
        }
    }

    // OLED timeout
    if (oledOn && (millis() - lastMotionTime >= 1000UL)) // 10s test
    //if (oledOn && (millis() - lastMotionTime >= 20UL * 60UL * 1000UL)) //20 min
    {
        oledOn = false;
        display_off();
    }

    // Update display continuously while OLED is on
    if (oledOn)
    {
        updateOLED();
    }
}


//void //publishDebugTiming(const char* functionName, uint32_t duration)
//{
//  char msg[64];
//
//  snprintf(
//    msg,
//    sizeof(msg),
//    "%s:%lums",
//    functionName,
//    duration
//  );
//
//  mqttClient.publish(
//    "server/bicker_ups/debug",
//    msg
//  );
//}

//void //publishDebugTiming(const char* functionName,uint32_t duration)
//{
//  if(duration < 50)
//    return;
//
//  char msg[64];
//
//  snprintf(
//    msg,
//    sizeof(msg),
//    "%s:%lums",
//    functionName,
//    duration
//  );
//
//  mqttClient.publish(MQTT_TOPIC_DEBUG,msg);
//}


//void //publishDebugTiming(const char* functionName,uint32_t duration)
//{
//  if(!mqttClient.connected())
//    return;
//
//  if(duration < 50)
//    return;
//
//  char msg[64];
//
//  snprintf(
//    msg,
//    sizeof(msg),
//    "%s:%lums",
//    functionName,
//    duration
//  );
//
//  mqttClient.publish(
//    MQTT_TOPIC_DEBUG,
//    msg
//  );
//}

//void //publishDebugTiming(const char* functionName,uint32_t duration)
//{
//  char msg[64];
//
//  snprintf(
//    msg,
//    sizeof(msg),
//    "%s:%lums",
//    functionName,
//    duration
//  );
//
//  mqttClient.publish(
//    MQTT_TOPIC_DEBUG,
//    msg
//  );
//}

void publishDebugTiming(const char* functionName,uint32_t duration)
{
  char topic[96];
  char msg[32];


  snprintf(
    topic,
    sizeof(topic),
    "%s/%s",
    MQTT_TOPIC_DEBUG,
    functionName
  );


  snprintf(
    msg,
    sizeof(msg),
    "%lums",
    duration
  );


  mqttClient.publish(
    topic,
    msg
  );
}

//void loop()
//{
//
// 
////  if(
////   millis()-lastLEDUpdate > 30){
////    lastLEDUpdate = millis();
////    updateLEDs();
////  }
////
////  if(millis()-lastOLEDUpdate >= 100){
////    lastOLEDUpdate = millis();
////    updateOLED();
////  }
//
//    handleOLEDTimeout();
//
//    if (millis() - lastLEDUpdate > 20)
//    {
//        lastLEDUpdate = millis();
//        updateLEDs();
//    }
//
//    if (oledOn && millis() - lastOLEDUpdate >= 100)
//    {
//        lastOLEDUpdate = millis();
//        updateOLED();
//    }
//
//  
//  wifi.loop();
//  nut.handle();
//  ota.loop();
//
//  
//
////  if(millis() - lastUpdate >= 1000){
//uint32_t interval = displayData.onBattery ? 500 : 5000;
//
//if(millis() - lastUpdate >= interval)
//{
//
//    lastUpdate = millis();
//
//
//
//    if(
//      powercap.update()
//    )
//    {
//
//      PowerCapData data =
//        powercap.getData();
//        displayData = data;
//
//
//
//   //   runtime.update(data);
//
//      runtime.update(
//        data.soc,
//        data.current,
//        data.onBattery,
//        data.state,
//        data.voltage
//      );
//
////      //oled_update();
////      oled.clear();  
////      //oled.println("USV");
////      oled.print("SOC: ");
////      oled.print(data.soc,0);
////      if(data.soc<100){
////        oled.println("  %");
////      }
////      else{
////      oled.println(" %");
////      }
////      oled.print("Backup: ");
////      oled.print(runtime.getRuntimeSeconds());
////      oled.println("s");
//  
//      //Serial.println();
//      //Serial.println("----------------------------");
//
//
//
//      //Serial.print("0x1B STATE = ");
//      //Serial.print(data.state);
//      //Serial.print("  ");
//      //Serial.println(data.stateText);
//
//
//
//      //Serial.print("CAP Voltage = ");
//      //Serial.print(data.voltage,3);
//      //Serial.println(" V");
//
//
//
//      //Serial.print("CAP Current = ");
//      //Serial.print(data.current,0);
//      //Serial.println(" mA");
//
//
//
//      //Serial.print("CAP Power = ");
//      //Serial.print(data.power,2);
//      //Serial.println(" W");
//
//
//
//      //Serial.print("SOC = ");
//      //Serial.print(data.soc,1);
//      //Serial.println(" %");
//
//
//
//      //Serial.print("Runtime = ");
//      //Serial.print(
//      //  runtime.getRuntimeSeconds()
//      //);
//      //Serial.println(" s");
//
//
//
//      nut.update(data,runtime);
//
//
//    }
//
//  }
//
//
//}


//void loop()
//{
//
//  uint32_t start;
//
//
//  start = millis();
//  handleOLEDTimeout();
//  //publishDebugTiming("handleOLEDTimeout",millis() - start);
//
//
//
//  if (millis() - lastLEDUpdate > 20)
//  {
//    lastLEDUpdate = millis();
//
//    start = millis();
//    updateLEDs();
//    //publishDebugTiming("updateLEDs",millis() - start);
//  }
//
//
//
//  if (oledOn && millis() - lastOLEDUpdate >= 100)
//  {
//    lastOLEDUpdate = millis();
//
//    start = millis();
//    updateOLED();
//    //publishDebugTiming("updateOLED",millis() - start);
//  }
//
//
//
//  start = millis();
//  wifi.loop();
//  //publishDebugTiming("wifi.loop",millis() - start);
//
//
//
//  start = millis();
//  nut.handle();
//  //publishDebugTiming("nut.handle",millis() - start);
//
//
//
//  start = millis();
//  ota.loop();
//  //publishDebugTiming("ota.loop",millis() - start);
//
//
//
//  uint32_t interval = displayData.onBattery ? 500 : 5000;
//
//
//  if(millis() - lastUpdate >= interval)
//  {
//
//    lastUpdate = millis();
//
//
//    start = millis();
//
//    bool updated = powercap.update();
//
//    //publishDebugTiming("powercap.update",millis() - start);
//
//
//
//    if(updated)
//    {
//
//      start = millis();
//
//      PowerCapData data = powercap.getData();
//      displayData = data;
//
//
//      runtime.update(
//        data.soc,
//        data.current,
//        data.onBattery,
//        data.state,
//        data.voltage
//      );
//
//      //publishDebugTiming("runtime.update",millis() - start);
//
//
//
//      start = millis();
//
//      nut.update(data,runtime);
//
//      //publishDebugTiming("nut.update",millis() - start);
//
//    }
//
//  }
//
//
//}


//void mqttReconnect()
//{
//  if(mqttClient.connected())
//    return;
//
//
//  Serial.println("MQTT connecting...");
//
//
//  if(
//    mqttClient.connect(
//      "Bicker_UPSIC-1205-ESP32",
//      MQTT_USER,
//      MQTT_PASSWORD
//    )
//  )
//  {
//    Serial.println("MQTT connected");
//  }
//  else
//  {
//    Serial.print("MQTT failed rc=");
//    Serial.println(mqttClient.state());
//  }
//}


//void mqttReconnect()
//{
//  if(mqttClient.connected())
//    return;
//
//
//  if(mqttClient.connect("Bicker_UPSIC-1205-ESP32",MQTT_USER,MQTT_PASSWORD)){
//    haDiscovery.publish();
//    mqttClient.publish(
//      MQTT_TOPIC_DEBUG,
//      "mqtt.connected"
//    );
//
//  }
//}

void mqttReconnect()
{
    if(mqttClient.connected())
        return;


    unsigned long now = millis();

    // try every 10 seconds only
    if(now - lastMQTTReconnect < 10000)
        return;


    lastMQTTReconnect = now;


    Serial.println("MQTT reconnect attempt");


    if(
        mqttClient.connect(
            "Bicker_UPSIC-1205-ESP32",
            MQTT_USER,
            MQTT_PASSWORD
        )
    )
    {
        Serial.println("MQTT connected");

        haDiscovery.publish();

        mqttClient.publish(
            MQTT_TOPIC_DEBUG,
            "mqtt.connected",
            true
        );
    }
    else
    {
        Serial.print("MQTT failed rc=");
        Serial.println(mqttClient.state());
    }
}


void loop()
{

  uint32_t start;


  start = millis();
  handleOLEDTimeout();
  //publishDebugTiming("handleOLEDTimeout",millis() - start);



  if (millis() - lastLEDUpdate > 20)
  {
    lastLEDUpdate = millis();

    start = millis();
    updateLEDs();
    //publishDebugTiming("updateLEDs",millis() - start);
  }



  if (oledOn && millis() - lastOLEDUpdate >= 100)
  {
    lastOLEDUpdate = millis();

    start = millis();
    updateOLED();
    //publishDebugTiming("updateOLED",millis() - start);
  }



  start = millis();
  wifi.loop();
  //publishDebugTiming("wifi.loop",millis() - start);



  start = millis();
  nut.handle();
  //publishDebugTiming("nut.handle",millis() - start);



  start = millis();
  ota.loop();
  //publishDebugTiming("ota.loop",millis() - start);



  start = millis();

//  if(!mqttClient.connected())
//  {
////    mqttClient.connect("Bicker_UPSIC-1205-ESP32");
//    mqttClient.connect(
//      "Bicker_UPSIC-1205-ESP32",
//      MQTT_USER,
//      MQTT_PASSWORD
//    );
//  }
//
//  //publishDebugTiming("mqtt.connect",millis() - start);
//
//
//
//  start = millis();
//
//  mqttClient.loop();
//
//  //publishDebugTiming("mqtt.loop",millis() - start);

//if(!mqttClient.connected())
//{
//  if(millis() - lastMQTTReconnect > 10000)
//  {
//    lastMQTTReconnect = millis();
//
//    start = millis();
//
//    mqttClient.connect(
//      "Bicker_UPSIC-1205-ESP32",
//      MQTT_USER,
//      MQTT_PASSWORD
//    );
//
//    //publishDebugTiming("mqtt.connect",millis() - start);
//  }
//}
start = millis();

mqttReconnect();

//publishDebugTiming("mqtt.connect",millis() - start);


start = millis();

mqttClient.loop();

//publishDebugTiming("mqtt.loop",millis() - start);

  uint32_t interval = displayData.onBattery ? 300 : 500;


  if(millis() - lastUpdate >= interval)
  {

    lastUpdate = millis();


    start = millis();

    bool updated = powercap.update();

    //publishDebugTiming("powercap.update",millis() - start);



    if(updated)
    {

      start = millis();

      PowerCapData data = powercap.getData();
      displayData = data;

      char buffer[32];

      snprintf(buffer,sizeof(buffer),"%.0f",data.soc);
      mqttClient.publish("server/bicker_ups/soc",buffer,true);
      
      snprintf(buffer,sizeof(buffer),"%.2f",data.voltage);
      mqttClient.publish("server/bicker_ups/voltage",buffer,true);
      
      snprintf(buffer,sizeof(buffer),"%.0f",data.current);
      mqttClient.publish("server/bicker_ups/current",buffer,true);
      
      snprintf(buffer,sizeof(buffer),"%.2f",data.power);
      mqttClient.publish("server/bicker_ups/power",buffer,true);
      
      snprintf(buffer,sizeof(buffer),"%lu",(unsigned long)runtime.getRuntimeSeconds());
      mqttClient.publish("server/bicker_ups/runtime",buffer,true);
      
      mqttClient.publish("server/bicker_ups/status",nut.getStatus().c_str(),true);

      runtime.update(
        data.soc,
        data.current,
        data.onBattery,
        data.state,
        data.voltage
      );

      //publishDebugTiming("runtime.update",millis() - start);



      start = millis();

      nut.update(data,runtime);

      //publishDebugTiming("nut.update",millis() - start);

    }

  }


}
