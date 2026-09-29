#ifndef HA_DISCOVERY_H
#define HA_DISCOVERY_H

#include <Arduino.h>
#include <PubSubClient.h>

class HADiscovery
{
public:

    void begin(PubSubClient &client);

    void publish();

private:

    PubSubClient *mqtt;

    void publishSensor(
        const char *id,
        const char *name,
        const char *unit,
        const char *deviceClass,
        const char *stateClass,
        const char *icon
    );

    bool published = false;
};

#endif
