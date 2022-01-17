#ifndef _LEDMATRIX_DEFINITIONS_H_
#define _LEDMATRIX_DEFINITIONS_H_

/*********** WS2812B leds *******************/

#define LED_PIN  D5 // leds pin

#define MATRIX_H 11
#define MATRIX_W 36

#define CURRENT_LIMIT 12000

uint8_t brightness = 100;

#define EFFECT_DURATION_SEC 60

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS

#include <FastLED.h>

CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/

#include <MatrixLineConverters.h>
#include "LEDGarland.hpp"

LEDGarland<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> ledMatrix;

void setup_LED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

#endif
