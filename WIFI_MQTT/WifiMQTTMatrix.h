#ifndef _WIFIMQTTMATRIX_H_
#define _WIFIMQTTMATRIX_H_

#include "WifiMQTTLine.h"

class WifiMQTTMatrix : public WifiMQTTLine
{
protected:

	Adafruit_MQTT_Subscribe runningString;

public:
	
	WifiMQTTMatrix();
};

#endif

