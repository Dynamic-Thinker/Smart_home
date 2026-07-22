/**************************************************************************
 *
 *  GwenHome v2.0
 *  File : esp_smarthome.ino
 *  Board: ESP-01 / ESP8266
 *  M:0,T:1,P:0,F:1,S:64
 **************************************************************************/

#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ArduinoOTA.h>

#include "webpage.h"
#include "serial_manager.h"
#include "state_manager.h"
#include "webserver.h"



/**************************************************************************
 *
 * WIFI
 *
 **************************************************************************/

const char* WIFI_SSID     = "Airtel_mohi_5691";
const char* WIFI_PASSWORD = "Air@16561";

const char* HOSTNAME = "gwenhome";

/**************************************************************************
 *
 * STATIC IP
 *
 **************************************************************************/

IPAddress LOCAL_IP(
    192,168,1,50
);

IPAddress GATEWAY(
    192,168,1,1
);

IPAddress SUBNET(
    255,255,255,0
);

IPAddress DNS(
    192,168,1,1
);

/**************************************************************************
 *
 * GLOBAL OBJECTS
 *
 **************************************************************************/

AsyncWebServer server(80);

AsyncEventSource events("/events");

/**************************************************************************
 *
 * GLOBALS
 *
 **************************************************************************/

bool wifiConnected = false;

unsigned long wifiReconnectTimer = 0;

const unsigned long WIFI_RECONNECT_INTERVAL = 5000;

/**************************************************************************
 *
 * WIFI
 *
 **************************************************************************/

void connectWiFi()
{
    WiFi.mode(WIFI_STA);

    WiFi.hostname(HOSTNAME);

    WiFi.config(
        LOCAL_IP,
        GATEWAY,
        SUBNET,
        DNS
    );

    WiFi.setAutoReconnect(true);

    WiFi.persistent(true);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    bool sentConnecting = false;
    while(WiFi.status()!=WL_CONNECTED)
    {
        if (!sentConnecting) {
            Serial.println("W:0");
            sentConnecting = true;
        }
        delay(1000);
    }

    wifiConnected = true;

    Serial.print("W:");
    Serial.println(WiFi.localIP().toString());
    delay(1000);
}

/**************************************************************************
 *
 * WIFI RECONNECT
 *
 **************************************************************************/

void checkWiFi()
{
    if(WiFi.status()==WL_CONNECTED)
    {
        if (!wifiConnected) {
            wifiConnected = true;
            Serial.print("W:");
            Serial.println(WiFi.localIP().toString());
            delay(1000);
            requestCurrentState();
        }
        return;
    }

    if (wifiConnected) {
        wifiConnected = false;
        Serial.println("W:0");
    }
    

    if(
        millis()-wifiReconnectTimer
        <
        WIFI_RECONNECT_INTERVAL
    )
        return;

    wifiReconnectTimer = millis();

    WiFi.reconnect();
}

/**************************************************************************
 *
 * OTA
 *
 **************************************************************************/

void initializeOTA()
{
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.begin();
}

/**************************************************************************
 *
 * INITIALIZATION
 *
 **************************************************************************/

void initializeSystem()
{
    connectWiFi();

    initializeState();

    initializeSerial();

    initializeWebServer();

    initializeOTA();

    requestCurrentState();
}

/**************************************************************************
 *
 * SETUP
 *
 **************************************************************************/

void setup()
{
    Serial.begin(9600);
    Serial.println();
    delay(1000);
    initializeSystem();
}

/**************************************************************************
 *
 * LOOP
 *
 **************************************************************************/

void loop()
{
    ArduinoOTA.handle();

    serialLoop();

    stateLoop();

    webServerLoop();

    checkWiFi();
}