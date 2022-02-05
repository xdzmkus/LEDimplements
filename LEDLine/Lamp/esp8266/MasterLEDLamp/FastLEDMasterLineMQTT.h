#ifndef _FASTLEDMASTERLINEMQTT_H_
#define _FASTLEDMASTERLINEMQTT_H_

#include "FastLEDLineMQTT.h"

#include "WifiMQTTLineMaster.h"

class FastLEDMasterLineMQTT : public FastLEDLineMQTT
{

public:

	FastLEDMasterLineMQTT(WifiMQTTLineMaster* wifiMqtt, LEDLine* ledLine, uint8_t startBrightness = 255, uint16_t effectDuration = 60);

	virtual void turnOnLeds() override;

	virtual void holdNextEffect() override;

protected:

	virtual void nextEffect() override;
};

#endif
