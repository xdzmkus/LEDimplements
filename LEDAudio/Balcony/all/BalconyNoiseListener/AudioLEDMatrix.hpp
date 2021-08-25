/*
* AudioLEDMatrix.hpp
*
*/

#ifndef _AUDIOLEDMATRIX_HPP_
#define _AUDIOLEDMATRIX_HPP_

#include <LEDLine.h>
#include <LEDAudioEffects.h>

void analyzeAudio(uint16_t& r, uint16_t& l);

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class AudioLEDMatrix : public LEDLine
{
private:

	static const uint8_t NUM_EFFECTS = 2;

	VUMeterMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> eff1;
	VUWaveMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> eff2;

	const char* const availableEffects[NUM_EFFECTS] = { eff1.name, eff2.name };

	ILedEffect* const effects[NUM_EFFECTS] = { &eff1, &eff2 };

public:

	AudioLEDMatrix() :
		eff1(20, 512, analyzeAudio),
		eff2(20, 256, analyzeAudio)
	{
	};

	virtual ~AudioLEDMatrix()
	{
	};

	virtual uint8_t howManyEffects() const override
	{
		return NUM_EFFECTS;
	};

	virtual const char* const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	virtual bool setEffectByName(const char* effectName) override
	{
		for (uint8_t idx = 0; idx < NUM_EFFECTS; ++idx)
		{
			if (strcmp(*(effects[idx]), effectName) == 0)
			{
				effect = effects[idx];
				effect->start();
				return true;
			}
		}

		return false;
	};
};

/*
template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
const uint8_t AudioLEDMatrix<leds, numLeds>::NUM_EFFECTS = 2;
*/

#endif
