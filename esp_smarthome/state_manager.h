#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include "serial_manager.h"

/********************************************************************
 * GwenHome State Manager
 ********************************************************************/

/*
    This module stores the latest Arduino state,
    notifies the web server whenever something changes
    and provides helper functions for the dashboard.
*/

/********************************************************************
 * Dashboard State
 ********************************************************************/

struct DashboardState
{
    bool connected;

    bool mainPower;

    bool tube;

    bool plug;

    bool fan;

    uint8_t speed;

    uint8_t cooler;

    unsigned long lastUpdate;
};

extern DashboardState dashboardState;

/********************************************************************
 * Initialization
 ********************************************************************/

void initializeState();

/********************************************************************
 * Main Loop
 ********************************************************************/

void stateLoop();

/********************************************************************
 * Update Functions
 ********************************************************************/

void updateStateFromArduino();

void markDisconnected();

void markConnected();

/********************************************************************
 * Getters
 ********************************************************************/

bool isConnected();

bool isMainPowerOn();

bool isTubeOn();

bool isPlugOn();

bool isFanOn();

uint8_t getFanSpeed();

const DashboardState& getDashboardState();

/********************************************************************
 * JSON
 ********************************************************************/

String getStateJSON();

/********************************************************************
 * Event Notification
 ********************************************************************/

bool hasDashboardChanged();

void clearDashboardChanged();

/********************************************************************
 * Utility
 ********************************************************************/

void copySerialState();

#endif