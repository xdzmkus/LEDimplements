#ifndef _LEDLINES_DEFINITIONS_H_
#define _LEDLINES_DEFINITIONS_H_

/*********** WS2812B leds *******************/

#define LED_PIN_1 12  // D6 - GPIO12 - left strip
#define LED_PIN_2 13  // D7 - GPIO13 - center strip
#define LED_PIN_3 14  // D5 - GPIO14 - right strip

#define RELAY_PIN 15  // D8 - GPIO15 - relay pin

#define NUM_LEDS_LEFT   15
#define NUM_LEDS_CENTER 24
#define NUM_LEDS_RIGHT  8

#define CURRENT_LIMIT 2000

#define START_BRIGHTNESS 50

#define EFFECT_DURATION_SEC 60

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS

#endif
