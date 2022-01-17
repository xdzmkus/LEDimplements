#include "WifiMQTT.h"

#include "SerialDebug.h"

extern void performAction_callback(uint32_t x);
extern void setEffect_callback(char* data, uint16_t len);

WifiMQTTClass::WifiMQTTClass()
{
	setEffect.setCallback(setEffect_callback);
	mqtt.subscribe(&setEffect);

	performAction.setCallback(performAction_callback);
	mqtt.subscribe(&performAction);

	mqttTicker.attach(60.0, pingMQTT_callback);
}

void WifiMQTTClass::init(bool configure)
{
	WiFiManager wm;

	if (configure)
	{
		wm.setConfigPortalTimeout(180);

		wm.setConfigPortalBlocking(true);
		if (wm.startConfigPortal(WLAN_SSID, WLAN_PASS))
		{
			log_println(F("WiFi Reconfigured! Rebooting..."));
			delay(BOOT_TIMEOUT);
			ESP.restart();
		}
	}

	// Set the ESP8266 to be a WiFi-client
	WiFi.mode(WIFI_STA);
	WiFi.hostname(WLAN_HOSTNAME);

	log_println(WLAN_HOSTNAME);

	if (!wm.autoConnect(WLAN_SSID, WLAN_PASS))
	{
		log_println(F("Connection Failed! Rebooting..."));
		delay(BOOT_TIMEOUT);
		ESP.restart();
	}

	log_println(F("WiFi connected"));
	log_print(F("IP address: "));
	log_println(WiFi.localIP());
}

void WifiMQTTClass::process()
{
	if (!keepAliveMQTT()) return;

	if (f_pingMQTT && mqtt.ping())
	{
		f_pingMQTT = false;

		log_println(F("Ping MQTT... "));
	}

	mqtt.processPackets(10);
}

void WifiMQTTClass::log(LOG_LEVEL level, String msg)
{
	String msg_keyword;
	String log_topic = MQTT_TOPIC_LOG;

	switch (level)
	{
	case LOG_LEVEL::INFO: {
		msg_keyword = "INFO: ";
		log_topic += "/info";
		break; }
	case LOG_LEVEL::WARN: {
		msg_keyword = "WARNING: ";
		log_topic += "/warning";
		break; }
	case LOG_LEVEL::ERROR: {
		msg_keyword = "ERROR: ";
		log_topic += "/error";
		break; }
	case LOG_LEVEL::DEBUG: {
		msg_keyword = "DEBUG: ";
		log_topic += "/debug";
		break; }
	default: {
		return; }
	}

	msg_keyword += msg;

	log_println(msg_keyword);

#ifndef MQTT_DEBUG
	if (level == LOG_LEVEL::DEBUG) return;
#endif

	Adafruit_MQTT_Publish mqttLog(&mqtt, log_topic.c_str());

	if (!keepAliveMQTT() || !mqttLog.publish(msg_keyword.c_str()))
	{
		log_println(F("Publish Message Failed"));
	}
}

bool WifiMQTTClass::keepAliveMQTT()
{
	if (!mqtt.connected())
	{
		log_println(F("Connecting to MQTT... "));

		uint8_t ret = mqtt.connect();

		if (ret != 0)
		{
			log_println(mqtt.connectErrorString(ret));
			log_println(F("Retry MQTT connection ..."));
			mqtt.disconnect();
			return false;
		}
		else
		{
			log_println(F("MQTT Connected!"));
			return true;
		}
	}

	return true;
}

void WifiMQTTClass::pingMQTT_callback()
{
	f_pingMQTT = true;
}

WifiMQTTClass WifiMQTT;

volatile bool WifiMQTTClass::f_pingMQTT = false;

WiFiClient              WifiMQTTClass::client;
Adafruit_MQTT_Client    WifiMQTTClass::mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Subscribe WifiMQTTClass::setEffect(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe WifiMQTTClass::performAction(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);



