#ifndef _WIFIMQTTLINEMASTER_H_
#define _WIFIMQTTLINEMASTER_H_

#include "WifiMQTTLine.h"

class WifiMQTTLineMaster : public WifiMQTTLine
{
private:

	static const char* const LOG_PUBLISH_FAIL PROGMEM;

protected:

	Adafruit_MQTT_Subscribe masterAction;

public:
	
	WifiMQTTLineMaster();

	void controlEffect(const char* effect);
};

#endif

