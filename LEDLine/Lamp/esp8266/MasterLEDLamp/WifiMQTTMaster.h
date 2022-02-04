#ifndef _WIFIMQTTMASTER_H_
#define _WIFIMQTTMASTER_H_

#include "WifiMQTT.h"

class WifiMQTTMaster : public WifiMQTT
{
public:
	
	WifiMQTTMaster();

	void pubMaster(const char* effect);
};

#endif

