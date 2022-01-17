#ifndef _LEDMATRIX_DEFINITIONS_H_
#define _LEDMATRIX_DEFINITIONS_H_

/*********** WS2812B leds *******************/

#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#else
#define LED_PIN  9 // leds pin
#endif

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000

uint8_t brightness = 30;

#define EFFECT_DURATION_SEC 60

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS

#include <FastLED.h>

CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/

#include <MatrixLineConverters.h>
#include "LEDPanel.hpp"

LEDPanel<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

void setup_LED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}


#endif
