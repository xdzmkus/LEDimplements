#ifndef _WIFIMQTTLINESLAVE_H_
#define _WIFIMQTTLINESLAVE_H_

#include "WifiMQTTLine.h"

class WifiMQTTLineSlave : public WifiMQTTLine
{
protected:

	Adafruit_MQTT_Subscribe slaveEffect;
	Adafruit_MQTT_Subscribe slaveAction;

public:
	
	WifiMQTTLineSlave();
};

#endif

