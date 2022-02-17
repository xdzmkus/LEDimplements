#ifndef _FASTLEDLINELOG_H_
#define _FASTLEDLINELOG_H_

#include "FastLEDLine.h"

class FastLEDLineLog : public FastLEDLine
{
public:

	FastLEDLineLog(LEDLine* ledLine, uint8_t startBrightness = 255, uint16_t effectDuration = 60);

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
