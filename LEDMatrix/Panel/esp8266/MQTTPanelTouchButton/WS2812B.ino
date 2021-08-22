/*********** WS2812B leds *******************/
#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#endif

#define MATRIX_H 8
#define MATRIX_W 32
#define CURRENT_LIMIT 8000

#define EFFECT_DURATION_SEC 45

uint8_t brightness = 100;

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include <ZigZagFromTopLeftToBottomAndRight.hpp>
#include "LEDMatrixEx.hpp"
LEDMatrixEx<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> ledMatrix;

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
    if (ledMatrix.refresh())
    {
        FastLED.show();
    }
}

void turnOnLeds()
{
	effectsTicker.detach();

	ledMatrix.setEffectByIdx(0);
	ledMatrix.turnOn();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledMatrix.turnOff();

	FastLED.clear(true);

	publishState();
}

void changeEffect()
{
	effectsTicker.detach();

	ledMatrix.setNextEffect();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();
}

void setEffect(const char* data)
{
	if (ledMatrix.setEffectByName(data))
	{
		effectsTicker.detach();
	}

	publishState();
}

const char* getEffect()
{
	return !ledMatrix.isOn() ? nullptr : ledMatrix.getEffectName();
}

void setRunningString(char* data, uint16_t len)
{
	ledMatrix.setRunningString(data, len);
}

void adjustBrightness(int8_t delta)
{
    brightness += delta;
    FastLED.setBrightness(brightness);

	Serial.print(F("BRIGHTNESS: "));
	Serial.println(brightness);
}
