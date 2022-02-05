#include "my_data_sensitive.h"

#include "SerialDebug.h"

#define RELAY_PIN D8 // GPIO15 - relay pin

#define LED_PIN_L D6 // GPIO12 - left strip
#define LED_PIN_C D7 // GPIO13 - center strip
#define LED_PIN_R D5 // GPIO14 - right strip

#define BTN_PIN   D0 // GPIO16 - touch button pin

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

#define NUM_LEDS_LEFT   15
#define NUM_LEDS_CENTER 24
#define NUM_LEDS_RIGHT  8

#define CURRENT_LIMIT 2000

#define START_BRIGHTNESS 80

#define EFFECT_DURATION_SEC 120

/*********** WIFI MQTT Manager ***************/

#include "WifiMQTTLineControl.h"
WifiMQTTLineControl wifiMqtt;

/*********** LED Line Effects ***************/

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS

#include <FastLED.h>
CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsCenter[NUM_LEDS_CENTER];
CRGB ledsRight[NUM_LEDS_RIGHT];

#include "LEDMultiLine.hpp"
LEDMultiLine<ledsLeft, NUM_LEDS_LEFT> ledLineLeft;
LEDMultiLine<ledsCenter, NUM_LEDS_CENTER> ledLineCenter;
LEDMultiLine<ledsRight, NUM_LEDS_RIGHT> ledLineRight;

#include "FastLEDThreeLinesMQTT.h"
FastLEDThreeLinesMQTT mqttLeds(&wifiMqtt, &ledLineLeft, &ledLineCenter, &ledLineRight, START_BRIGHTNESS, EFFECT_DURATION_SEC);

/********** Button module ***********/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

#include <EventsQueue.hpp>
EventsQueue<BUTTON_EVENT, 10> queue;

#include <Ticker.h>
Ticker btnTicker;

Ticker builtinLedTicker;

/********************************************/

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN_L, GRB>(ledsLeft, NUM_LEDS_LEFT).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_C, GRB>(ledsCenter, NUM_LEDS_CENTER).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_R, GRB>(ledsRight, NUM_LEDS_RIGHT).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
}

IRAM_ATTR void blinkLED()
{
	//toggle LED state
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

IRAM_ATTR void check_button()
{
	btn.check();
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	queue.push(eventType);
}

void processBtn()
{
	bool processBtnEvent;
	BUTTON_EVENT btnEvent;

	do
	{
		noInterrupts();

		processBtnEvent = queue.length();

		if (processBtnEvent)
		{
			btnEvent = queue.pop();
		}

		interrupts();

		if (processBtnEvent)
		{
			switch (btnEvent)
			{
			case BUTTON_EVENT::Clicked:
				mqttLeds.holdNextEffect();
				digitalWrite(RELAY_PIN, HIGH);
				wifiMqtt.controlAction(NEXT_CODE);
				break;
			case BUTTON_EVENT::DoubleClicked:
				mqttLeds.turnOnLeds();
				digitalWrite(RELAY_PIN, HIGH);
				wifiMqtt.controlAction(ON_CODE);
				break;
			case BUTTON_EVENT::RepeatClicked:
				mqttLeds.adjustBrightness(-10);
				wifiMqtt.controlAction(FastLED.getBrightness());
				break;
			case BUTTON_EVENT::LongPressed:
				mqttLeds.turnOffLeds();
				digitalWrite(RELAY_PIN, LOW);
				wifiMqtt.controlAction(OFF_CODE);
				break;
			default:
				return;
			}
		}

	} while (processBtnEvent);
}

void setAction_callback(uint32_t x)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new action requested = ")) + String(x));

	if (x <= 255)
	{
		mqttLeds.setBrightness(x);
		return;
	}

	switch (x)
	{
	case ON_CODE:
		mqttLeds.turnOnLeds();
		digitalWrite(RELAY_PIN, HIGH);
		break;
	case OFF_CODE:
		mqttLeds.turnOffLeds();
		digitalWrite(RELAY_PIN, LOW);
		break;
	case NEXT_CODE:
		mqttLeds.holdNextEffect();
		digitalWrite(RELAY_PIN, HIGH);
		break;
	default:
		break;
	}
}

void setEffect_callback(char* data, uint16_t len)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new effect requested = ")) + String(data));

	if (mqttLeds.holdEffectByName(data))
	{
		digitalWrite(RELAY_PIN, HIGH);
	}
}

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

	pinMode(RELAY_PIN, OUTPUT);          // Initialize the RELAY pin as an output
	digitalWrite(RELAY_PIN, LOW);        // Turn off leds

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_FastLED();

	mqttLeds.setup();

	btn.initPin();

	delay(wifiMqtt.BOOT_TIMEOUT);

	builtinLedTicker.attach_ms(500, blinkLED);  // Blink led while setup

	bool f_setupMode = btn.check();

	if (f_setupMode) log_println(F("BUTTON PRESSED - RECONFIGURE WIFI"));

	wifiMqtt.init(f_setupMode);

	btn.setEventHandler(handleButtonEvent);

	btnTicker.attach_ms(btn.delayDebounce >> 1, check_button);

	builtinLedTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	mqttLeds.turnOnLeds();
	digitalWrite(RELAY_PIN, HIGH);
}

void loop()
{
	processBtn();

	wifiMqtt.process();

	mqttLeds.process();
}
