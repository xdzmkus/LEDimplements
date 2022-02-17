#include "WifiMQTTMatrix.h"

#include "my_data_sensitive.h"

#if !defined(MQTT_TOPIC_RUNNING_STRING)
#pragma message "Default credentials are used"
#endif

#ifndef MQTT_TOPIC_RUNNING_STRING
#define MQTT_TOPIC_RUNNING_STRING   MQTT_USERNAME"/set/runningString"
#endif 

#include "SerialDebug.h"

extern void setRunningString_callback(char* data, uint16_t len);

WifiMQTTMatrix::WifiMQTTMatrix()
	: WifiMQTTLine(), runningString(&mqtt, MQTT_TOPIC_RUNNING_STRING, MQTT_QOS_1)
{
	runningString.setCallback(setRunningString_callback);
	mqtt.subscribe(&runningString);
}

