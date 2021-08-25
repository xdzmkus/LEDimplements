/*********** WS2812B leds *******************/
#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#else
#define LED_PIN  9  // leds pin
#endif

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 8000

#define EFFECT_DURATION_SEC 60

uint8_t brightness = 100;

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include <ZigZagFromTopLeftToBottomAndRight.hpp>
#include "AudioLEDMatrix.hpp"
AudioLEDMatrix<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> ledAudio;

#include <Ticker.h>
Ticker effectsTicker;

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
}

void processLED()
{
    if (ledAudio.refresh())
    {
        FastLED.show();
    }
}

void turnOnLeds()
{
	effectsTicker.detach();

	ledAudio.setEffectByIdx(0);
	ledAudio.turnOn();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledAudio.turnOff();

	FastLED.clear(true);
}

void changeEffect()
{
	effectsTicker.detach();

	ledAudio.setNextEffect();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);
}

void setEffect(const char* data)
{
	if (ledAudio.setEffectByName(data))
	{
		effectsTicker.detach();
	}
}

const char* getEffect()
{
	return !ledAudio.isOn() ? nullptr : ledAudio.getEffectName();
}


void adjustBrightness(int8_t delta)
{
    brightness += delta;
    FastLED.setBrightness(brightness);
}
