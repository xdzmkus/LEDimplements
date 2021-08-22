#if true //__has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else
#pragma message "default credentials are used"

#define WLAN_SSID           "AP wifi name"
#define WLAN_PASS           "and password"
#define WLAN_HOSTNAME       "set hostname"

#define MQTT_SERVER         "127.0.0.1"
#define MQTT_SERVERPORT     1883
#define MQTT_USERNAME       "your mqtt username"
#define MQTT_KEY            "and password"

#define MQTT_TOPIC_PUB      MQTT_USERNAME"/master/state"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/set/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/set/action"

#define ON_CODE             6735
#define OFF_CODE            2344
#define NEXT_CODE           2747

#define OTA_USERNAME       "your username"
#define OTA_KEY            "and password"
#define OTA_UPDATE_PATH    "/firmware"

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDSzCCAjMCCQD2ahcfZAwXxDANBgkqhkiG9w0BAQsFADCBiTELMAkGA1UEBhMC
VVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFjAUBgNVBAcMDU9yYW5nZSBDb3VudHkx
EDAOBgNVBAoMB1ByaXZhZG8xGjAYBgNVBAMMEXNlcnZlci56bGFiZWwuY29tMR8w
HQYJKoZIhvcNAQkBFhBlYXJsZUB6bGFiZWwuY29tMB4XDTE4MDMwNjA1NDg0NFoX
DTE5MDMwNjA1NDg0NFowRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3Rh
dGUxITAfBgNVBAoMGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAPVKBwbZ+KDSl40YCDkP6y8Sv4iNGvEOZg8Y
X7sGvf/xZH7UiCBWPFIRpNmDSaZ3yjsmFqm6sLiYSGSdrBCFqdt9NTp2r7hga6Sj
oASSZY4B9pf+GblDy5m10KDx90BFKXdPMCLT+o76Nx9PpCvw13A848wHNG3bpBgI
t+w/vJCX3bkRn8yEYAU6GdMbYe7v446hX3kY5UmgeJFr9xz1kq6AzYrMt/UHhNzO
S+QckJaY0OGWvmTNspY3xCbbFtIDkCdBS8CZAw+itnofvnWWKQEXlt6otPh5njwy
+O1t/Q+Z7OMDYQaH02IQx3188/kW3FzOY32knER1uzjmRO+jhA8CAwEAATANBgkq
hkiG9w0BAQsFAAOCAQEAnDrROGRETB0woIcI1+acY1yRq4yAcH2/hdq2MoM+DCyM
E8CJaOznGR9ND0ImWpTZqomHOUkOBpvu7u315blQZcLbL1LfHJGRTCHVhvVrcyEb
fWTnRtAQdlirUm/obwXIitoz64VSbIVzcqqfg9C6ZREB9JbEX98/9Wp2gVY+31oC
JfUvYadSYxh3nblvA4OL+iEZiW8NE3hbW6WPXxvS7Euge0uWMPc4uEcnsE0ZVG3m
+TGimzSdeWDvGBRWZHXczC2zD4aoE5vrl+GD2i++c6yjL/otHfYyUpzUfbI2hMAA
5tAF1D5vAAwA8nfPysumlLsIjohJZo4lgnhB++AlOg==
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA9UoHBtn4oNKXjRgIOQ/rLxK/iI0a8Q5mDxhfuwa9//FkftSI
IFY8UhGk2YNJpnfKOyYWqbqwuJhIZJ2sEIWp2301OnavuGBrpKOgBJJljgH2l/4Z
uUPLmbXQoPH3QEUpd08wItP6jvo3H0+kK/DXcDzjzAc0bdukGAi37D+8kJfduRGf
zIRgBToZ0xth7u/jjqFfeRjlSaB4kWv3HPWSroDNisy39QeE3M5L5ByQlpjQ4Za+
ZM2yljfEJtsW0gOQJ0FLwJkDD6K2eh++dZYpAReW3qi0+HmePDL47W39D5ns4wNh
BofTYhDHfXzz+RbcXM5jfaScRHW7OOZE76OEDwIDAQABAoIBAQDKov5NFbNFQNR8
djcM1O7Is6dRaqiwLeH4ZH1pZ3d9QnFwKanPdQ5eCj9yhfhJMrr5xEyCqT0nMn7T
yEIGYDXjontfsf8WxWkH2TjvrfWBrHOIOx4LJEvFzyLsYxiMmtZXvy6YByD+Dw2M
q2GH/24rRdI2klkozIOyazluTXU8yOsSGxHr/aOa9/sZISgLmaGOOuKI/3Zqjdhr
eHeSqoQFt3xXa8jw01YubQUDw/4cv9rk2ytTdAoQUimiKtgtjsggpP1LTq4xcuqN
d4jWhTcnorWpbD2cVLxrEbnSR3VuBCJEZv5axg5ZPxLEnlcId8vMtvTRb5nzzszn
geYUWDPhAoGBAPyKVNqqwQl44oIeiuRM2FYenMt4voVaz3ExJX2JysrG0jtCPv+Y
84R6Cv3nfITz3EZDWp5sW3OwoGr77lF7Tv9tD6BptEmgBeuca3SHIdhG2MR+tLyx
/tkIAarxQcTGsZaSqra3gXOJCMz9h2P5dxpdU+0yeMmOEnAqgQ8qtNBfAoGBAPim
RAtnrd0WSlCgqVGYFCvDh1kD5QTNbZc+1PcBHbVV45EmJ2fLXnlDeplIZJdYxmzu
DMOxZBYgfeLY9exje00eZJNSj/csjJQqiRftrbvYY7m5njX1kM5K8x4HlynQTDkg
rtKO0YZJxxmjRTbFGMegh1SLlFLRIMtehNhOgipRAoGBAPnEEpJGCS9GGLfaX0HW
YqwiEK8Il12q57mqgsq7ag7NPwWOymHesxHV5mMh/Dw+NyBi4xAGWRh9mtrUmeqK
iyICik773Gxo0RIqnPgd4jJWN3N3YWeynzulOIkJnSNx5BforOCTc3uCD2s2YB5X
jx1LKoNQxLeLRN8cmpIWicf/AoGBANjRSsZTKwV9WWIDJoHyxav/vPb+8WYFp8lZ
zaRxQbGM6nn4NiZI7OF62N3uhWB/1c7IqTK/bVHqFTuJCrCNcsgld3gLZ2QWYaMV
kCPgaj1BjHw4AmB0+EcajfKilcqtSroJ6MfMJ6IclVOizkjbByeTsE4lxDmPCDSt
/9MKanBxAoGAY9xo741Pn9WUxDyRplww606ccdNf/ksHWNc/Y2B5SPwxxSnIq8nO
j01SmsCUYVFAgZVOTiiycakjYLzxlc6p8BxSVqy6LlJqn95N8OXoQ+bkwUux/ekg
gz5JWYhbD6c38khSzJb0pNXCo3EuYAVa36kDM96k1BtWuhRS10Q1VXk=
-----END RSA PRIVATE KEY-----
)EOF";

#endif

/*********** WiFi ***************************/
#include <WiFiManager.h>

/*********** OTA  ***************************/
#include <ESP8266mDNS.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServerSecure httpServer(443);
ESP8266HTTPUpdateServerSecure httpUpdater;
BearSSL::X509List chain(serverCert);
BearSSL::PrivateKey pk(serverKey);

/*********** MQTT Server ********************/
#include <WiFiClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client    mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

// Setup MQTT topics
Adafruit_MQTT_Publish   mqttClientState(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe mqttClientEffect(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe mqttClientAction(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

#include <Ticker.h>
Ticker mqttTicker;

volatile boolean f_publishState = true;

void mqtt_setAction_callback(uint32_t x)
{
	Serial.print(F("new action requested: "));
	Serial.println(x);

	switch (x)
	{
	case ON_CODE:
		turnOnLeds();
		break;
	case OFF_CODE:
		turnOffLeds();
		break;
	case NEXT_CODE:
		changeEffect();
		break;
	default:
		break;
	}
}

void mqtt_setEffect_callback(char* data, uint16_t len)
{
	Serial.print(F("new effect requested: "));
	Serial.println(data);

	setEffect(data);
}

void configure_WiFi()
{
    WiFiManager wm;

    wm.setConfigPortalTimeout(180);

    wm.setConfigPortalBlocking(true);
    if (wm.startConfigPortal(WLAN_SSID, WLAN_PASS))
    {
        Serial.println(F("WiFi Reconfigured! Rebooting..."));
        delay(5000);
        ESP.restart();
    }
}

void setup_WiFi()
{
    WiFiManager wm;

    WiFi.mode(WIFI_STA);                // Set the ESP8266 to be a WiFi-client
    WiFi.hostname(WLAN_HOSTNAME);

    Serial.println(WLAN_HOSTNAME);

    if (!wm.autoConnect(WLAN_SSID, WLAN_PASS))
    {
        Serial.println(F("Connection Failed! Rebooting..."));
        delay(5000);
        ESP.restart();
    }

    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_OTA()
{
	configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

	MDNS.begin(WLAN_HOSTNAME);

	httpServer.getServer().setRSACert(&chain, &pk);
	httpUpdater.setup(&httpServer, OTA_UPDATE_PATH, OTA_USERNAME, OTA_KEY);
	httpServer.begin();

	MDNS.addService("https", "tcp", 443);
}

void processOTA()
{
	MDNS.update();

	httpServer.handleClient();
}

void setup_MQTT()
{
    mqttClientEffect.setCallback(mqtt_setEffect_callback);
    mqtt.subscribe(&mqttClientEffect);

    mqttClientAction.setCallback(mqtt_setAction_callback);
    mqtt.subscribe(&mqttClientAction);

    mqttTicker.attach_scheduled(10.0, pingMQTT);
}

void pingMQTT()
{
	if (!mqtt.connected())
	{
		uint8_t ret(mqtt.connect());

		Serial.println(F("Connecting to MQTT... "));
		if (ret != 0)
		{
			Serial.println(mqtt.connectErrorString(ret));
			Serial.println(F("Retry MQTT connection ..."));
			mqtt.disconnect();
			return;
		}
		else
		{
			Serial.println(F("MQTT Connected!"));
		}
	}

	mqtt.ping();
}

void processMQTT()
{
	 mqtt.processPackets(10);

	if (f_publishState && mqtt.connected())
	{
		String summaryEffect;
		const char* currentEffect;

		currentEffect = getEffect(0);
		if (currentEffect == nullptr) summaryEffect += "OFF";
		else  summaryEffect += currentEffect;

		currentEffect = getEffect(1);
		if (currentEffect == nullptr) summaryEffect += "OFF";
		else  summaryEffect += currentEffect;

		currentEffect = getEffect(2);
		if (currentEffect == nullptr) summaryEffect += "OFF";
		else  summaryEffect += currentEffect;

		Serial.print(F("Publish message: "));
		Serial.println(summaryEffect);

		if (!mqttClientState.publish(summaryEffect.c_str()))
		{
			Serial.println(F("Publish Message Failed"));
			return;
		}

		f_publishState = false;
	}
}

void publishState()
{
	f_publishState = true;
}
