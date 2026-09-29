#ifndef CONFIG_H
#define CONFIG_H


// ===============================
// WiFi
// ===============================

#define WIFI_SSID       "YOUR-WIFI-SSID"
#define WIFI_PASSWORD   "YOUR-WIFI-PASSWORD"

#define MQTT_SERVER "192.168.178.2"
#define MQTT_PORT   1883
#define MQTT_TOPIC_DEBUG "server/bicker_ups/debug"
#define MQTT_USER     "bicker"
#define MQTT_PASSWORD "YOURpassword"


#define WIFI_HOSTNAME   "Bicker_UPSIC-1205-esp32"
#define OTA_HOSTNAME    "Bicker_UPSIC-1205-esp32"


// ===============================
// UPS Identification
// ===============================

#define UPS_NAME        "UPSIC1205"
#define UPS_VENDOR      "BICKER"
#define UPS_MODEL       "UPSIC-1205"

// ===============================
// I2C OLED
// ===============================
// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS_OLED 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1



#define PIR_pin 2
// ===============================
// Neopixel
// ===============================
#define LED_PIN      1
#define NUM_LEDS     12
#define LEDstrip_global_brightness_setting 50 //range: 0-255
#define flashBrightness 255

extern uint8_t ledBrightness;

// ===============================
// I2C PowerCap
// ===============================

#define I2C_ADDRESS     0x09


// ESP32 I2C Pins
#define I2C_SDA         4
#define I2C_SCL         3



// ===============================
// PowerCap Registers
// ===============================

#define REG_STATE       0x1B

#define REG_SOC_RAW     0x25

#define REG_CAP_RAW     0x26

#define REG_ESR1        0x27
#define REG_ESR2        0x28

#define REG_CURRENT     0x29

#define REG_MISC        0x2A



// ===============================
// Voltage conversion
//
// Determined from measurements:
//
// RAW 3487 = 5.719V
// RAW 4960 = 8.134V
//
// factor ≈ 0.00164 V/count
//
// ===============================

#define CAP_VOLT_FACTOR 0.00164f



// ===============================
// Current conversion
//
// Raw signed values:
// -4053 -> discharge
// +16294 -> charge
//
// scale to mA
// ===============================

#define CURRENT_FACTOR  1.0f



// ===============================
// SOC mapping
//
// Based on measured discharge curve
//
// 8.57V  = 90%
// 8.13V  = charging region
// 5.72V  = lower battery
// 1.63V  = shutdown
//
// ===============================

#define SOC_FULL_VOLTAGE       8.57f
#define SOC_EMPTY_VOLTAGE      1.63f



// Shutdown threshold
// Hardware shutdown happened at:
// approx 1.6V

#define SOC_SHUTDOWN_PERCENT   0.0f



// ===============================
// NUT Server
// ===============================

#define NUT_PORT 3493


#endif
