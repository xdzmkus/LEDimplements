#ifndef _FASTLEDTHREELINESMQTT_H_
#define _FASTLEDTHREELINESMQTT_H_

#include "FastLEDThreeLines.h"

#include "WifiMQTT.h"

class FastLEDThreeLinesMQTT : public FastLEDThreeLines
{
protected:

	WifiMQTT* wifiMqtt;

public:

	FastLEDThreeLinesMQTT(WifiMQTT* wifiMqtt, LEDLine* ledLineLeft, LEDLine* ledLineCenter, LEDLine* ledLineRight, uint8_t startBrightness = 255, uint16_t effectDuration = 60);

	virtual void adjustBrightness(int8_t delta) override;

	virtual void setBrightness(uint8_t newBrightness) override;

	virtual void turnOnLeds() override;

	virtual void turnOffLeds() override;

	virtual void holdNextEffect() override;

	virtual bool holdEffectByName(char* data) override;

protected:

	virtual void nextEffect() override;
};

#endif
