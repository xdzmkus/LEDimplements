#ifndef _WIFIMQTTLINECONTROL_H_
#define _WIFIMQTTLINECONTROL_H_

#include "WifiMQTTLine.h"

class WifiMQTTLineControl : public WifiMQTTLine
{
private:

	static const char* const LOG_PUBLISH_FAIL PROGMEM;

public:
	
	WifiMQTTLineControl();

	void controlEffect(const char* effect);

	void controlAction(uint32_t action);
};

#endif

