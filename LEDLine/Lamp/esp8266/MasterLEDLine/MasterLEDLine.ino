#define BTN_PIN   16  // D0 - GPIO16 - touch button pin

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

#include <Ticker.h>
Ticker ledTicker;

boolean f_setupMode = false;

/********** Touch button module *************/
#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
    switch (eventType)
    {
    case BUTTON_EVENT::Clicked:
        changeEffect();
        break;
    case BUTTON_EVENT::DoubleClicked:
        turnOnLeds();
        break;
    case BUTTON_EVENT::RepeatClicked:
        adjustBrightness(-10);
        break;
    case BUTTON_EVENT::LongPressed:
        turnOffLeds();
        break;
    default:
        return;
    }
}

void blinkLED()
{
    //toggle LED state
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
    Serial.begin(115200);

    randomSeed(analogRead(UNPINNED_ANALOG_PIN));

    setup_LED();

    pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
    digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

    btn.initPin();

    delay(5000);

    ledTicker.attach_ms(500, blinkLED);  // Blink led while setup

    f_setupMode = btn.check();

    if (f_setupMode)
    {
        configure_WiFi();
        setup_WiFi();
        setup_OTA();
        turnOnFlashLeds();
    }
    else
    {
        setup_WiFi();
        setup_MQTT();
        turnOnLeds();
    }

    btn.setEventHandler(handleButtonEvent);

    ledTicker.detach();
    digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH
}


void loop()
{
    btn.check();

    if (f_setupMode)
    {
        processOTA();
    }
    else
    {
        processMQTT();
    }

    processLED();
}
