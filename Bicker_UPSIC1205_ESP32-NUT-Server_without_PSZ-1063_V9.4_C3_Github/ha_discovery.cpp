//#include "ha_discovery.h"
//#include "config.h"
//
//void HADiscovery::begin(PubSubClient &client)
//{
//    mqtt = &client;
//}
//
//void HADiscovery::publish()
//{
//    if(published)
//        return;
//
//    publishSensor(
//        "soc",
//        "UPS Charge",
//        "%",
//        "battery",
//        "measurement",
//        "mdi:battery"
//    );
//
//    publishSensor(
//        "voltage",
//        "UPS Voltage",
//        "V",
//        "voltage",
//        "measurement",
//        "mdi:flash"
//    );
//
//    publishSensor(
//        "current",
//        "UPS Current",
//        "mA",
//        "current",
//        "measurement",
//        "mdi:current-dc"
//    );
//
//    publishSensor(
//        "power",
//        "UPS Power",
//        "W",
//        "power",
//        "measurement",
//        "mdi:flash"
//    );
//
//    publishSensor(
//        "runtime",
//        "UPS Runtime",
//        "s",
//        "duration",
//        "measurement",
//        "mdi:timer-outline"
//    );
//
//    publishSensor(
//        "status",
//        "UPS Status",
//        "",
//        "",
//        "",
//        "mdi:power-plug"
//    );
//
//    published = true;
//}
//
//
////void HADiscovery::publish()
////{
////  mqtt->publish("server/test/discovery","hello",true);
////}
//
//
//void HADiscovery::publishSensor(
//    const char *id,
//    const char *name,
//    const char *unit,
//    const char *deviceClass,
//    const char *stateClass,
//    const char *icon
//)
//{
//    char topic[128];
//    char payload[1024];
//
//    snprintf(
//        topic,
//        sizeof(topic),
//        //"homeassistant/sensor/%s/%s/config",
//        "homeassistant/sensor/%s_%s/config",
//        WIFI_HOSTNAME,
//        id
//    );
//
//    snprintf(
//        payload,
//        sizeof(payload),
//        "{"
//        "\"name\":\"%s\","
//        "\"uniq_id\":\"%s_%s\","
//        "\"stat_t\":\"server/bicker_ups/%s\","
//        "\"unit_of_meas\":\"%s\","
//        "\"dev_cla\":\"%s\","
//        "\"stat_cla\":\"%s\","
//        "\"ic\":\"%s\","
//        "\"dev\":{"
//            "\"ids\":[\"%s\"],"
//            "\"name\":\"Bicker UPSIC-1205\","
//            "\"mdl\":\"UPSIC-1205\","
//            "\"mf\":\"Bicker\""
//        "}"
//        "}",
//        name,
//        WIFI_HOSTNAME,
//        id,
//        id,
//        unit,
//        deviceClass,
//        stateClass,
//        icon,
//        WIFI_HOSTNAME
//    );
//
//    mqtt->publish(topic,payload,true);
//}


#include "ha_discovery.h"
#include "config.h"

void HADiscovery::begin(PubSubClient &client)
{
    mqtt = &client;
}

void HADiscovery::publish()
{
    if(published)
        return;

    publishSensor(
        "soc",
        "UPS Charge",
        "%",
        "battery",
        "measurement",
        "mdi:battery"
    );

    publishSensor(
        "voltage",
        "UPS Voltage",
        "V",
        "voltage",
        "measurement",
        "mdi:flash"
    );

    publishSensor(
        "current",
        "UPS Current",
        "mA",
        "current",
        "measurement",
        "mdi:current-dc"
    );

    publishSensor(
        "power",
        "UPS Power",
        "W",
        "power",
        "measurement",
        "mdi:flash"
    );

    publishSensor(
        "runtime",
        "UPS Runtime",
        "s",
        "duration",
        "measurement",
        "mdi:timer-outline"
    );

    publishSensor(
        "status",
        "UPS Status",
        "",
        "",
        "",
        "mdi:power-plug"
    );

    published = true;
}


void HADiscovery::publishSensor(
    const char *id,
    const char *name,
    const char *unit,
    const char *deviceClass,
    const char *stateClass,
    const char *icon
)
{
    char topic[128];
    char payload[1024];

    snprintf(
        topic,
        sizeof(topic),
        "homeassistant/sensor/%s_%s/config",
        WIFI_HOSTNAME,
        id
    );

    snprintf(
        payload,
        sizeof(payload),
        "{"
        "\"name\":\"%s\","
        "\"has_entity_name\":true,"
        "\"object_id\":\"%s\","
        "\"uniq_id\":\"%s_%s\","
        "\"stat_t\":\"server/bicker_ups/%s\","
        "\"unit_of_meas\":\"%s\","
        "\"dev_cla\":\"%s\","
        "\"stat_cla\":\"%s\","
        "\"ic\":\"%s\","
        "\"dev\":{"
            "\"ids\":[\"%s\"],"
            "\"name\":\"Bicker UPSIC-1205\","
            "\"mdl\":\"UPSIC-1205\","
            "\"mf\":\"Bicker\""
        "}"
        "}",
        name,
        id,
        WIFI_HOSTNAME,
        id,
        id,
        unit,
        deviceClass,
        stateClass,
        icon,
        WIFI_HOSTNAME
    );

    mqtt->publish(topic, payload, true);
}
