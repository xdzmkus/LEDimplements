#if defined(ESP8266)
#define BTN_PIN  D0 // button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin
#define MIC_PIN  A0 // not connected analog pin
#elif defined(ESP32)
#define BTN_PIN  5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define MIC_PIN  35 // not connected analog pin
#else
#define BTN_PIN  4  // button pin
#define ENC1_PIN 3  // encoder S1 pin
#define ENC2_PIN 2	// encoder S2 pin
#define MIC_PIN  A0 // not connected analog pin
#endif

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 8> queue;

#if defined(ESP8266)
IRAM_ATTR
#endif
void catchEncoderTicks()
{
    encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
    queue.push(eventType);
}

void processEncoder()
{
    bool processEncEvent;
    ENCODER_EVENT encEvent;

    do
    {
        noInterrupts();

        processEncEvent = queue.length();

        if (processEncEvent)
        {
            encEvent = queue.pop();
        }

        interrupts();

        if (processEncEvent)
        {
            switch (encEvent)
            {
            case ENCODER_EVENT::LEFT:
                adjustBrightness(-5);
                break;
            case ENCODER_EVENT::RIGHT:
                adjustBrightness(5);
                break;
            default:
                break;
            }
        }
    } while (processEncEvent);
}

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
    case BUTTON_EVENT::LongPressed:
        turnOffLeds();
        break;
    default:
        break;
    }
}

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup()
{

#ifdef ADCSRA

    // поднимаем частоту опроса аналогового порта до 38.4 к√ц, по теореме
    //  отельникова (Ќайквиста) максимальна€ частота дискретизации будет 19 к√ц
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

#endif // ADCSRA

//    analogReference(INTERNAL);

    setup_LED();

    btn.initPin();

    btn.setEventHandler(handleButtonEvent);

    encoder.initPins();

    encoder.setEventHandler(handleEncoderEvent);

    attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

    turnOnLeds();
}

void loop()
{
    btn.check();

    processEncoder();

    processLED();
}

void analyzeAudio(uint16_t& r, uint16_t& l)
{
    uint16_t rMin = -1; // MAX_UINT16_T

    for (uint16_t i = 0; i < 256; i++)
    {
        uint16_t value = analogRead(MIC_PIN);

        if (value > r) r = value;
        if (value < rMin) rMin = value;
    }

    r -= rMin;
    l = r;
}
