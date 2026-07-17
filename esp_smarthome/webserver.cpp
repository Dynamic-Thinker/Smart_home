#include "webserver.h"

unsigned long lastHeartbeat = 0;

/*********************************************************************
 *
 * INITIALIZATION
 *
 *********************************************************************/

void initializeWebServer()
{
    DefaultHeaders::Instance().addHeader(
        "Access-Control-Allow-Origin",
        "*"
    );

    registerRoutes();

    initializeEvents();

    server.begin();
}

/*********************************************************************
 *
 * ROUTES
 *
 *********************************************************************/

void registerRoutes()
{
    server.on(
        "/",
        HTTP_GET,
        handleDashboard
    );

    server.on(
        "/status",
        HTTP_GET,
        handleStatus
    );

    server.on(
        "/toggle",
        HTTP_POST,
        handleToggle
    );

    server.on(
        "/speed",
        HTTP_POST,
        handleSpeed
    );

    server.on(
        "/cooler",
        HTTP_POST,
        handleCooler
    );

    server.on(
        "/manifest.json",
        HTTP_GET,
        handleManifest
    );

    server.on(
        "/favicon.ico",
        HTTP_GET,
        handleFavicon
    );

    server.onNotFound(
        [](AsyncWebServerRequest *request)
        {
            request->send(
                404,
                "text/plain",
                "Not Found"
            );
        }
    );
}

/*********************************************************************
 *
 * EVENTS (SSE)
 *
 *********************************************************************/

void initializeEvents()
{
    events.onConnect(onClientConnect);

    server.addHandler(&events);
}

/********************************************************************/

void onClientConnect(
    AsyncEventSourceClient *client)
{
    if (client->lastId())
    {
        // Reconnected client
    }

    String json = getStateJSON();

    client->send(
        json.c_str(),
        "state",
        millis()
    );
}

/*********************************************************************
 *
 * LOOP
 *
 *********************************************************************/

void webServerLoop()
{
    if (hasDashboardChanged())
    {
        notifyClients();

        clearDashboardChanged();
    }

    pushHeartbeat();
}

/*********************************************************************
 *
 * NOTIFY ALL CLIENTS
 *
 *********************************************************************/

void notifyClients()
{
    String json = getStateJSON();

    events.send(
        json.c_str(),
        "state",
        millis()
    );
}

/********************************************************************/

void pushHeartbeat()
{
    if (millis() - lastHeartbeat < 15000)
        return;

    lastHeartbeat = millis();

    events.send(
        "alive",
        "ping",
        millis()
    );
}

/*********************************************************************
 *
 * DASHBOARD
 *
 *********************************************************************/

void handleDashboard(
    AsyncWebServerRequest *request)
{
    request->send_P(
        200,
        "text/html",
        INDEX_HTML
    );
}

/*********************************************************************
 *
 * STATUS
 *
 *********************************************************************/

void handleStatus(
    AsyncWebServerRequest *request)
{
    sendJSON(
        request,
        getStateJSON()
    );
}

/*********************************************************************
 *
 * TOGGLE
 *
 *********************************************************************/

void handleToggle(
    AsyncWebServerRequest *request)
{
    String dev =
        requestDevice(request);

    if (dev == "main")
        toggleMainPower();

    else if (dev == "tube")
        toggleTube();

    else if (dev == "plug")
        togglePlug();

    else if (dev == "fan")
        toggleFan();

    else
    {
        sendError(
            request,
            400,
            "Invalid device"
        );

        return;
    }

    sendOK(request);
}

/*********************************************************************
 *
 * FAN SPEED
 *
 *********************************************************************/

void handleSpeed(
    AsyncWebServerRequest *request)
{
    uint8_t speed =
        requestSpeed(request);

    setFanSpeed(speed);

    sendOK(request);
}

/*********************************************************************
 *
 * COOLER
 *
 *********************************************************************/

void handleCooler(
    AsyncWebServerRequest *request)
{
    uint8_t cooler =
        requestCooler(request);

    setCoolerState(cooler);

    sendOK(request);
}

/*********************************************************************
 *
 * HELPERS
 *
 *********************************************************************/

String requestDevice(
    AsyncWebServerRequest *request)
{
    if (request->hasParam("d", true))
    {
        return request->getParam(
            "d", true
        )->value();
    }

    return "";
}

/********************************************************************/

uint8_t requestSpeed(
    AsyncWebServerRequest *request)
{
    if (!request->hasParam("v", true))
        return 0;

    int s =
        request->getParam(
            "v", true
        )->value().toInt();

    if (s < 0)
        s = 0;

    if (s > 100)
        s = 100;

    return (uint8_t)s;
}

/********************************************************************/

uint8_t requestCooler(
    AsyncWebServerRequest *request)
{
    if (!request->hasParam("v", true))
        return 2;

    int c =
        request->getParam(
            "v", true
        )->value().toInt();

    if (c < 0 || c > 2)
        c = 2; // Default to auto if invalid

    return (uint8_t)c;
}

/********************************************************************/

void sendOK(
    AsyncWebServerRequest *request)
{
    request->send(
        200,
        "text/plain",
        "OK"
    );
}

/********************************************************************/

void sendJSON(
    AsyncWebServerRequest *request,
    const String &json)
{
    request->send(
        200,
        "application/json",
        json
    );
}

/********************************************************************/

void sendError(
    AsyncWebServerRequest *request,
    int code,
    const String &message)
{
    request->send(
        code,
        "text/plain",
        message
    );
}

/*********************************************************************
 *
 * STATIC FILES
 *
 *********************************************************************/

void handleManifest(
    AsyncWebServerRequest *request)
{
    request->send(
        200,
        "application/json",
        "{}"
    );
}

/********************************************************************/

void handleFavicon(
    AsyncWebServerRequest *request)
{
    request->send(
        204,
        "image/x-icon",
        ""
    );
}