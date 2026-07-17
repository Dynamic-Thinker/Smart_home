#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <Arduino.h>

/********************************************************************
 * GwenHome Serial Manager
 ********************************************************************/

/*
ESP  -------> Arduino

t  -> Toggle Tube
p  -> Toggle Plug
f  -> Toggle Fan
m  -> Toggle Main Power

sFanSpeed  -> Set Fan Speed
             Website: 0-100%
             ESP maps: 0..100 -> 128..0
             Sends mapped decimal ASCII value

?  -> Request Current State

Arduino Reply

M:1,T:1,P:0,F:1,S:64

S value is raw (0-128), ESP maps back to 0-100%

*/

struct HomeState
{
    bool mainPower;

    bool tube;

    bool plug;

    bool fan;

    uint8_t speed;

    uint8_t cooler;
};

extern HomeState homeState;

/********************************************************************
 * Globals
 ********************************************************************/

extern String serialBuffer;

extern bool stateUpdated;

/********************************************************************
 * Initialization
 ********************************************************************/

void initializeSerial();

/********************************************************************
 * Main Loop
 ********************************************************************/

void serialLoop();

/********************************************************************
 * Commands To Arduino
 ********************************************************************/

void requestCurrentState();

void toggleMainPower();

void toggleTube();

void togglePlug();

void toggleFan();

void setFanSpeed(uint8_t speed);

void setCoolerState(uint8_t state);

/********************************************************************
 * Incoming Parser
 ********************************************************************/

void parseSerialLine(const String &line);

bool parseStatusPacket(const String &packet);

bool parsePair(
    const String &key,
    const String &value
);

/********************************************************************
 * Outgoing Helpers
 ********************************************************************/

void sendFullState();

/********************************************************************
 * State
 ********************************************************************/

bool hasStateChanged();

void clearStateFlag();

const HomeState& getCurrentState();

#endif