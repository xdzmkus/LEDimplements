#pragma once

#ifndef _MINIMALLEDMATRIX_HPP_
#define _MINIMALLEDMATRIX_HPP_

#include <LEDLine.h>
#include <UniversalLEDMatrixEffects.h>
#include <StaticLEDLineEffects.h>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class MinimalLEDMatrix : public LEDLine
{
private:

	static const uint8_t NUM_EFFECTS = 4;

	SparklesLedEffect<ledLine, width * height> eff1;
	GravityMatrixLedEffect<MATRIX, ledLine, width, height> eff2;
	StarfallMatrixLedEffect<MATRIX, ledLine, width, height> eff3;
	SinusMatrixLedEffect<MATRIX, ledLine, width, height> eff4;


	const char* const availableEffects[NUM_EFFECTS] = { eff1.name, eff2.name, eff3.name, eff4.name };

	ILedEffect* const effects[NUM_EFFECTS] = { &eff1, &eff2, &eff3, &eff4 };

public:

	MinimalLEDMatrix() :
		eff1(20, CRGB::Aquamarine),
		eff2(5), 
		eff3(10, CRGB::LightGreen),
		eff4(50)
	{
	};

	virtual ~MinimalLEDMatrix()
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

#endif
