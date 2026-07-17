#include "state_manager.h"

DashboardState dashboardState =
{
    false,      // connected
    false,      // mainPower
    false,      // tube
    false,      // plug
    false,      // fan
    0,          // speed
    2,          // cooler
    0           // lastUpdate
};

static bool dashboardChanged = false;

/********************************************************************/

void initializeState()
{
    dashboardState.connected = false;
    dashboardState.mainPower = false;
    dashboardState.tube = false;
    dashboardState.plug = false;
    dashboardState.fan = false;
    dashboardState.speed = 0;
    dashboardState.cooler = 2;
    dashboardState.lastUpdate = millis();

    dashboardChanged = true;
}

/********************************************************************/

void stateLoop()
{
    if (hasStateChanged())
    {
        updateStateFromArduino();
        clearStateFlag();
    }

    if (millis() - dashboardState.lastUpdate > 30000)
    {
        dashboardState.connected = false;
    }
}

/********************************************************************/

void updateStateFromArduino()
{
    copySerialState();

    dashboardState.connected = true;
    dashboardState.lastUpdate = millis();

    dashboardChanged = true;
}

/********************************************************************/

void copySerialState()
{
    const HomeState &s = getCurrentState();

    dashboardState.mainPower = s.mainPower;
    dashboardState.tube = s.tube;
    dashboardState.plug = s.plug;
    dashboardState.fan = s.fan;
    dashboardState.speed = s.speed;
    dashboardState.cooler = s.cooler;
}

/********************************************************************/

void markConnected()
{
    dashboardState.connected = true;
    dashboardState.lastUpdate = millis();

    dashboardChanged = true;
}

/********************************************************************/

void markDisconnected()
{
    dashboardState.connected = false;

    dashboardChanged = true;
}

/********************************************************************/

bool isConnected()
{
    return dashboardState.connected;
}

/********************************************************************/

bool isMainPowerOn()
{
    return dashboardState.mainPower;
}

/********************************************************************/

bool isTubeOn()
{
    return dashboardState.tube;
}

/********************************************************************/

bool isPlugOn()
{
    return dashboardState.plug;
}

/********************************************************************/

bool isFanOn()
{
    return dashboardState.fan;
}

/********************************************************************/

uint8_t getFanSpeed()
{
    return dashboardState.speed;
}

/********************************************************************/

const DashboardState& getDashboardState()
{
    return dashboardState;
}

/********************************************************************/

String getStateJSON()
{
    String json = "{";

    json += "\"connected\":";
    json += dashboardState.connected ? "true," : "false,";

    json += "\"main\":";
    json += dashboardState.mainPower ? "true," : "false,";

    json += "\"tube\":";
    json += dashboardState.tube ? "true," : "false,";

    json += "\"plug\":";
    json += dashboardState.plug ? "true," : "false,";

    json += "\"fan\":";
    json += dashboardState.fan ? "true," : "false,";

    json += "\"speed\":";
    json += String(dashboardState.speed);
    json += ",";

    json += "\"cooler\":";
    json += String(dashboardState.cooler);

    json += "}";

    return json;
}

/********************************************************************/

bool hasDashboardChanged()
{
    return dashboardChanged;
}

/********************************************************************/

void clearDashboardChanged()
{
    dashboardChanged = false;
}