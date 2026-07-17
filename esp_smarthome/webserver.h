#ifndef GWEN_WEBSERVER_H
#define GWEN_WEBSERVER_H

/**************************************************************************
 *
 * GwenHome v2.0
 * Async Web Server
 *
 **************************************************************************/

#include <Arduino.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "serial_manager.h"
#include "state_manager.h"
#include "webpage.h"

/**************************************************************************
 *
 * GLOBALS
 *
 **************************************************************************/

extern AsyncWebServer server;

extern AsyncEventSource events;

/**************************************************************************
 *
 * INITIALIZATION
 *
 **************************************************************************/

void initializeWebServer();

void registerRoutes();

void initializeEvents();

/**************************************************************************
 *
 * LOOP
 *
 **************************************************************************/

void webServerLoop();

/**************************************************************************
 *
 * DASHBOARD
 *
 **************************************************************************/

void handleDashboard(
    AsyncWebServerRequest *request
);

void handleStatus(
    AsyncWebServerRequest *request
);

/**************************************************************************
 *
 * DEVICE CONTROL
 *
 **************************************************************************/

void handleToggle(
    AsyncWebServerRequest *request
);

void handleSpeed(
    AsyncWebServerRequest *request
);

void handleCooler(
    AsyncWebServerRequest *request
);

/**************************************************************************
 *
 * EVENTS
 *
 **************************************************************************/

void notifyClients();

void pushHeartbeat();

void onClientConnect(
    AsyncEventSourceClient *client
);

/**************************************************************************
 *
 * HELPERS
 *
 **************************************************************************/

String requestDevice(
    AsyncWebServerRequest *request
);

uint8_t requestSpeed(
    AsyncWebServerRequest *request
);

uint8_t requestCooler(
    AsyncWebServerRequest *request
);

void sendOK(
    AsyncWebServerRequest *request
);

void sendJSON(
    AsyncWebServerRequest *request,
    const String &json
);

void sendError(
    AsyncWebServerRequest *request,
    int code,
    const String &message
);

/**************************************************************************
 *
 * STATIC FILES
 *
 **************************************************************************/

void handleManifest(
    AsyncWebServerRequest *request
);

void handleFavicon(
    AsyncWebServerRequest *request
);



#endif // GWEN_WEBSERVER_H