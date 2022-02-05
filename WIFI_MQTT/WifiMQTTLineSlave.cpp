#include "WifiMQTTLineSlave.h"

#include "my_data_sensitive.h"

#if !defined(MQTT_TOPIC_CONTROL_EFFECT) || !defined(MQTT_TOPIC_CONTROL_ACTION)
#pragma message "Default credentials are used"
#endif

#ifndef MQTT_TOPIC_CONTROL_EFFECT
#define MQTT_TOPIC_CONTROL_EFFECT   MQTT_USERNAME"/control/effect"
#endif 

#ifndef MQTT_TOPIC_CONTROL_ACTION
#define MQTT_TOPIC_CONTROL_ACTION   MQTT_USERNAME"/control/action"
#endif 

#include "SerialDebug.h"

extern void setAction_callback(uint32_t x);
extern void setEffect_callback(char* data, uint16_t len);

WifiMQTTLineSlave::WifiMQTTLineSlave()
	: slaveEffect(&mqtt, MQTT_TOPIC_CONTROL_EFFECT, MQTT_QOS_1), slaveAction(&mqtt, MQTT_TOPIC_CONTROL_ACTION, MQTT_QOS_1)
{
	slaveEffect.setCallback(setEffect_callback);
	mqtt.subscribe(&slaveEffect);

	slaveAction.setCallback(setAction_callback);
	mqtt.subscribe(&slaveAction);
}

