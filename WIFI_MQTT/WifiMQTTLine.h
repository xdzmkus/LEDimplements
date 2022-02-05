#ifndef _WIFIMQTTLINE_H_
#define _WIFIMQTTLINE_H_

#include "WifiMQTT.h"

class WifiMQTTLine : public WifiMQTT
{
protected:

	Adafruit_MQTT_Subscribe setEffect;
	Adafruit_MQTT_Subscribe setAction;

public:

	WifiMQTTLine();

};

#endif

