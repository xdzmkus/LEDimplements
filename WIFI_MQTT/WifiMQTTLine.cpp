#include "WifiMQTTLine.h"

#include "my_data_sensitive.h"

#if !defined(MQTT_TOPIC_SET_EFFECT) || !defined(MQTT_TOPIC_SET_ACTION) || !defined(ON_CODE) || !defined(OFF_CODE) || !defined(NEXT_CODE)
#pragma message "Default credentials are used"
#endif

#ifndef MQTT_TOPIC_SET_EFFECT
#define MQTT_TOPIC_SET_EFFECT     MQTT_USERNAME"/set/effect"
#endif

#ifndef MQTT_TOPIC_SET_ACTION
#define MQTT_TOPIC_SET_ACTION     MQTT_USERNAME"/set/action"
#endif

#ifndef ON_CODE
#define ON_CODE     6735
#endif

#ifndef OFF_CODE
#define OFF_CODE    2344
#endif

#ifndef NEXT_CODE
#define NEXT_CODE   2747
#endif

extern void setAction_callback(uint32_t x);
extern void setEffect_callback(char* data, uint16_t len);

WifiMQTTLine::WifiMQTTLine()
	: setEffect(&mqtt, MQTT_TOPIC_SET_EFFECT, MQTT_QOS_1), setAction(&mqtt, MQTT_TOPIC_SET_ACTION, MQTT_QOS_1)
{
	setEffect.setCallback(setEffect_callback);
	mqtt.subscribe(&setEffect);

	setAction.setCallback(setAction_callback);
	mqtt.subscribe(&setAction);
}

