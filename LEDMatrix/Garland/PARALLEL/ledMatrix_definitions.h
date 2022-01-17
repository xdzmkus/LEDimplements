#ifndef _LEDMATRIX_DEFINITIONS_H_
#define _LEDMATRIX_DEFINITIONS_H_

/*********** WS2812B leds *******************/

#define LED_PIN1  5   // leds pin
#define LED_PIN2  6   // leds pin
#define LED_PIN3  7   // leds pin
#define LED_PIN4  8   // leds pin
#define LED_PIN5  11  // leds pin
#define LED_PIN6  12  // leds pin
#define LED_PIN7  9   // leds pin
#define LED_PIN8  10  // leds pin

#define MATRIX_H 30
#define MATRIX_W 8

#define CURRENT_LIMIT 15000

uint8_t brightness = 100;

#define EFFECT_DURATION_SEC 60

#include <FastLED.h>

CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/

#include <MatrixLineConverters.h>
#include "LEDGarland.hpp"

LEDGarland<ParallelFromBottomRightToLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

void setup_LED()
{
	FastLED.addLeds<WS2812B, LED_PIN1, GRB>(leds + 0 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds + 1 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN3, GRB>(leds + 2 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN4, GRB>(leds + 3 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN5, GRB>(leds + 4 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN6, GRB>(leds + 5 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN7, GRB>(leds + 6 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN8, GRB>(leds + 7 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);

	FastLED.clear(true);
}

#endif
