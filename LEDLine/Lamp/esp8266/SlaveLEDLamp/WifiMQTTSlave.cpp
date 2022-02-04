#include "WifiMQTTSlave.h"

#if true //__has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else
#pragma message "default credentials are used"
#define MQTT_TOPIC_MASTER   MQTT_USERNAME"/master/effect"
#endif

extern void setEffect_callback(char* data, uint16_t len);

WifiMQTTSlave::WifiMQTTSlave()
{
	masterEffect.setCallback(setEffect_callback);
	mqtt.subscribe(&masterEffect);
}

Adafruit_MQTT_Subscribe WifiMQTTSlave::masterEffect(&mqtt, MQTT_TOPIC_MASTER, MQTT_QOS_1);
