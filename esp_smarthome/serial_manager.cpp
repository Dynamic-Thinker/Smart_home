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

char serialBuffer[64];
int serialIndex = 0;

bool stateUpdated = false;

/********************************************************************/

void initializeSerial()
{
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
            serialBuffer[serialIndex] = '\0';
            parseSerialLine(serialBuffer);
            serialIndex = 0;
        }
        else
        {
            if (serialIndex < 63) {
                serialBuffer[serialIndex++] = c;
            }
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
    Serial.print((int)round(128.0 - (homeState.speed * 1.28)));
    Serial.print(",C:");
    Serial.println(homeState.cooler);
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
    Serial.print("T:"); Serial.print(targetState ? 1 : 0);
    Serial.print(",P:"); Serial.print(targetState ? 1 : 0);
    Serial.print(",F:"); Serial.print(targetState ? 1 : 0);
    Serial.print('\n');
}

/********************************************************************/

void toggleTube()
{
    homeState.tube = !homeState.tube;
    updateMainPowerState();
    stateUpdated = true;
    Serial.print("T:"); Serial.println(homeState.tube ? 1 : 0);
}

/********************************************************************/

void togglePlug()
{
    homeState.plug = !homeState.plug;
    updateMainPowerState();
    stateUpdated = true;
    Serial.print("P:"); Serial.println(homeState.plug ? 1 : 0);
}

/********************************************************************/

void toggleFan()
{
    homeState.fan = !homeState.fan;
    updateMainPowerState();
    stateUpdated = true;
    Serial.print("F:"); Serial.println(homeState.fan ? 1 : 0);
}

/********************************************************************/

void setFanSpeed(uint8_t percent)
{
    if (percent > 100)
        percent = 100;

    homeState.speed = percent;
    stateUpdated = true;
    Serial.print("S:"); Serial.println((int)round(128.0 - (homeState.speed * 1.28)));
}

/********************************************************************/

void setCoolerState(uint8_t state)
{
    homeState.cooler = state;
    stateUpdated = true;
    Serial.print("C:"); Serial.println(homeState.cooler);
}

/********************************************************************/

void parseSerialLine(char *line)
{
    if (strlen(line) == 0)
        return;

    parseStatusPacket(line);
}

/********************************************************************/

bool parseStatusPacket(char *packet)
{
    char *pair = strtok(packet, ",");
    while (pair != NULL) {
        char *colon = strchr(pair, ':');
        if (colon != NULL) {
            *colon = '\0';
            char *key = pair;
            char *value = colon + 1;
            parsePair(key, value);
        }
        pair = strtok(NULL, ",");
    }

    updateMainPowerState();
    stateUpdated = true;

    return true;
}

/********************************************************************/

bool parsePair(
    const char *key,
    const char *value)
{
    int v = atoi(value);

    if (key[0] == 'M' && key[1] == '\0')
    {
        homeState.mainPower = v;
        return true;
    }

    if (key[0] == 'T' && key[1] == '\0')
    {
        homeState.tube = v;
        return true;
    }

    if (key[0] == 'P' && key[1] == '\0')
    {
        homeState.plug = v;
        return true;
    }

    if (key[0] == 'F' && key[1] == '\0')
    {
        homeState.fan = v;
        return true;
    }

    if (key[0] == 'S' && key[1] == '\0')
    {
        homeState.speed = (uint8_t)round((128.0 - v) * 100.0 / 128.0);
        return true;
    }

    if (key[0] == 'C' && key[1] == '\0')
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