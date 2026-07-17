#include "serial_manager.h"
#include <ESP8266WiFi.h>
HomeState homeState =
{
    false,
    false,
    false,
    false,
    0,
    2 // Cooler: 2 = auto
};

String serialBuffer = "";

bool stateUpdated = false;

/********************************************************************/

void initializeSerial()
{
    serialBuffer.reserve(64);
}

/********************************************************************/

void serialLoop()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '\r')
            continue;

        if (c == '\n')
        {
            parseSerialLine(serialBuffer);
            serialBuffer = "";
        }
        else
        {
            serialBuffer += c;
        }
    }
}

/********************************************************************/

void requestCurrentState()
{
    Serial.println("?");
}

/********************************************************************/

void updateMainPowerState()
{
    homeState.mainPower = (homeState.tube || homeState.plug || homeState.fan);
}

/********************************************************************/

void sendFullState()
{
    bool allOn = (homeState.tube && homeState.plug && homeState.fan);

    Serial.print("M:");
    Serial.print(allOn ? 1 : 0);
    Serial.print(",T:");
    Serial.print(homeState.tube ? 1 : 0);
    Serial.print(",P:");
    Serial.print(homeState.plug ? 1 : 0);
    Serial.print(",F:");
    Serial.print(homeState.fan ? 1 : 0);
    Serial.print(",S:");
    Serial.print(map(homeState.speed, 0, 100, 128, 0));
    Serial.print(",C:");
    Serial.print(homeState.cooler);
    Serial.print(",W:");
    
    extern bool wifiConnected;
    if (wifiConnected) {
        Serial.println(WiFi.localIP().toString());
    } else {
        Serial.println("0");
    }
}

/********************************************************************/

void toggleMainPower()
{
    bool targetState = !homeState.mainPower;
    
    homeState.tube = targetState;
    homeState.plug = targetState;
    homeState.fan = targetState;
    homeState.mainPower = targetState;
    
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void toggleTube()
{
    homeState.tube = !homeState.tube;
    updateMainPowerState();
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void togglePlug()
{
    homeState.plug = !homeState.plug;
    updateMainPowerState();
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void toggleFan()
{
    homeState.fan = !homeState.fan;
    updateMainPowerState();
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void setFanSpeed(uint8_t percent)
{
    if (percent > 100)
        percent = 100;

    homeState.speed = percent;
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void setCoolerState(uint8_t state)
{
    homeState.cooler = state;
    stateUpdated = true;
    sendFullState();
}

/********************************************************************/

void parseSerialLine(const String &line)
{
    if (line.length() == 0)
        return;

    parseStatusPacket(line);
}

/********************************************************************/

bool parseStatusPacket(const String &packet)
{
    int start = 0;

    while (start < packet.length())
    {
        int comma = packet.indexOf(',', start);

        if (comma == -1)
            comma = packet.length();

        String pair = packet.substring(start, comma);

        int colon = pair.indexOf(':');

        if (colon != -1)
        {
            String key =
                pair.substring(0, colon);

            String value =
                pair.substring(colon + 1);

            key.trim();
            value.trim();

            parsePair(key, value);
        }

        start = comma + 1;
    }

    updateMainPowerState();
    stateUpdated = true;

    return true;
}

/********************************************************************/

bool parsePair(
    const String &key,
    const String &value)
{
    int v = value.toInt();

    if (key == "M")
    {
        homeState.mainPower = v;
        return true;
    }

    if (key == "T")
    {
        homeState.tube = v;
        return true;
    }

    if (key == "P")
    {
        homeState.plug = v;
        return true;
    }

    if (key == "F")
    {
        homeState.fan = v;
        return true;
    }

    if (key == "S")
    {
        homeState.speed =
            map(v, 128, 0, 0, 100);

        return true;
    }

    if (key == "C")
    {
        homeState.cooler = v;
        return true;
    }

    return false;
}

/********************************************************************/

bool hasStateChanged()
{
    return stateUpdated;
}

/********************************************************************/

void clearStateFlag()
{
    stateUpdated = false;
}

/********************************************************************/

const HomeState& getCurrentState()
{
    return homeState;
}