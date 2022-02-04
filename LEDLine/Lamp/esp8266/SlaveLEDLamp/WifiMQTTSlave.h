#ifndef _WIFIMQTTSLAVE_H_
#define _WIFIMQTTSLAVE_H_

#include "WifiMQTT.h"

class WifiMQTTSlave : public WifiMQTT
{
protected:

	static Adafruit_MQTT_Subscribe masterEffect;

public:
	
	WifiMQTTSlave();
};

#endif

