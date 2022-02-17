#include "my_data_sensitive.h"

#include "SerialDebug.h"

#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#define BTN_PIN  D0 // button pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define BTN_PIN  5  // button pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000
#define START_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 60

#include <Ticker.h>
Ticker builtinLedTicker;
Ticker btnTicker;

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <EventsQueue.hpp>
EventsQueue<BUTTON_EVENT, 16> queue;

/*********** WIFI MQTT Manager ***************/

#include "WifiMQTTMatrix.h"
WifiMQTTMatrix wifiMqtt;

/*********** LED Matrix Effects *************/

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <MatrixLineConverters.h>
#include "LEDPanel.hpp"

LEDPanel<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

#include "FastLEDLineMQTT.h"
FastLEDLineMQTT mqttLeds(&wifiMqtt, &ledMatrix, START_BRIGHTNESS, EFFECT_DURATION_SEC);

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
}

/********************************************/

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
	btn.check();

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
				break;
			case BUTTON_EVENT::DoubleClicked:
				mqttLeds.turnOnLeds();
				break;
			case BUTTON_EVENT::RepeatClicked:
				mqttLeds.adjustBrightness(-START_BRIGHTNESS);
				break;
			case BUTTON_EVENT::LongPressed:
				mqttLeds.turnOffLeds();
				break;
			default:
				break;
			}
		}

	} while (processBtnEvent);
}

void setAction_callback(uint32_t x)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new action requested = ")) + String(x));

	switch (x)
	{
	case ON_CODE:
		mqttLeds.turnOnLeds();
		break;
	case OFF_CODE:
		mqttLeds.turnOffLeds();
		break;
	case NEXT_CODE:
		mqttLeds.holdNextEffect();
		break;
	default:
		break;
	}
}

void setEffect_callback(char* data, uint16_t len)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new effect requested = ")) + String(data));

	mqttLeds.holdEffectByName(data);
}

void setRunningString_callback(char* data, uint16_t len)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new running string received = ")) + String(data));

	ledMatrix.setRunningString(data, len);

	wifiMqtt.log(LOG_LEVEL::INFO, String(F("RUNNING_STRING = ")) + String(data));
}

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

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
}

void loop()
{
	processBtn();

	wifiMqtt.process();

	mqttLeds.process();
}
