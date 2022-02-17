#include "WifiMQTTLineMaster.h"

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

const char* const WifiMQTTLineMaster::LOG_PUBLISH_FAIL PROGMEM = "Publish Control Message Failed";

WifiMQTTLineMaster::WifiMQTTLineMaster()
	: masterAction(&mqtt, MQTT_TOPIC_CONTROL_ACTION, MQTT_QOS_1)
{
	masterAction.setCallback(setAction_callback);
	mqtt.subscribe(&masterAction);
}

void WifiMQTTLineMaster::controlEffect(const char* effect)
{
	Adafruit_MQTT_Publish mqttControl(&mqtt, MQTT_TOPIC_CONTROL_EFFECT);

	if (!keepAliveMQTT() || !mqttControl.publish(effect))
	{
		log_println(LOG_PUBLISH_FAIL);
	}
}


