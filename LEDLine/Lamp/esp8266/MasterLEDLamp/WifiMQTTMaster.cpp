#include "WifiMQTTMaster.h"

#define SERIAL_DEBUG
#include "SerialDebug.h"

#if true //__has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else
#pragma message "default credentials are used"
#define MQTT_TOPIC_MASTER   MQTT_USERNAME"/master/effect"
#endif

WifiMQTTMaster::WifiMQTTMaster()
{
}

void WifiMQTTMaster::pubMaster(const char* effect)
{
	Adafruit_MQTT_Publish mqttMaster(&mqtt, MQTT_TOPIC_MASTER);

	if (!keepAliveMQTT() || !mqttMaster.publish(effect))
	{
		log_println(F("Publish Master Message Failed"));
	}
}