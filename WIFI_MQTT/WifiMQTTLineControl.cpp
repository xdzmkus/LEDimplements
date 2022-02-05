#include "WifiMQTTLineControl.h"

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

const char* const WifiMQTTLineControl::LOG_PUBLISH_FAIL PROGMEM = "Publish Control Message Failed";

WifiMQTTLineControl::WifiMQTTLineControl()
{
}

void WifiMQTTLineControl::controlEffect(const char* effect)
{
	Adafruit_MQTT_Publish mqttControl(&mqtt, MQTT_TOPIC_CONTROL_EFFECT);

	if (!keepAliveMQTT() || !mqttControl.publish(effect))
	{
		log_println(LOG_PUBLISH_FAIL);
	}
}

void WifiMQTTLineControl::controlAction(uint32_t action)
{
	Adafruit_MQTT_Publish mqttControl(&mqtt, MQTT_TOPIC_CONTROL_ACTION);

	if (!keepAliveMQTT() || !mqttControl.publish(action))
	{
		log_println(LOG_PUBLISH_FAIL);
	}
}

