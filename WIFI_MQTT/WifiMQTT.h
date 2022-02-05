#ifndef _WIFIMQTT_H_
#define _WIFIMQTT_H_

#include <Arduino.h>

#include <WiFiManager.h>

#include <WiFiClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include <Ticker.h>

enum class LOG_LEVEL
{
	INFO,
	WARN,
	ERROR,
	DEBUG
};

class WifiMQTT
{
private:

	volatile static bool f_pingMQTT;

	Ticker mqttTicker;

protected:

	WiFiClient client;

	Adafruit_MQTT_Client mqtt;

public:

	static const uint16_t BOOT_TIMEOUT = 5000;

	WifiMQTT();

	virtual ~WifiMQTT();

	void init(bool configure = false);

	void process();

	void log(LOG_LEVEL level, String msg);

protected:
	
	bool keepAliveMQTT();

	static void pingMQTT_callback();
};

#endif

