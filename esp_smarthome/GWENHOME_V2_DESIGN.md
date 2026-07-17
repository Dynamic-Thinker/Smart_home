# GWENHOME_V2_DESIGN

## Project Overview
GwenHome is a smart-home controller split across two processors.

- **Arduino UNO**: controls relays, IR remote, rotary encoder, fan regulator, and is the authoritative source of appliance state.
- **ESP-01 (ESP8266)**: provides Wi-Fi, web dashboard, REST API, OTA, and synchronizes state between clients and the Arduino.

## Architecture

Browser / Phone / Raspberry Pi Gwen
        |
   Wi-Fi Router
        |
     ESP8266
        |
      UART
        |
    Arduino UNO
        |
 Relays / Fan / IR / Encoder

## Design Principles

- Arduino owns hardware.
- ESP owns networking.
- Browser only displays Arduino-confirmed state.
- Multiple clients must stay synchronized.
- Raspberry Pi Gwen communicates only via HTTP.

## UART Protocol

ESP -> Arduino
- t : Toggle tube
- p : Toggle plug
- f : Toggle fan
- a : Toggle main power
- ? : Request current state

Fan speed:
- Website: 0-100%
- ESP maps 0..100 -> 128..0
- ESP sends decimal ASCII value.

Arduino -> ESP

M:1,T:0,P:1,F:1,S:64

## REST API

GET /status

Returns:
{
 connected,
 main,
 tube,
 plug,
 fan,
 speed
}

POST /toggle
d=main|tube|plug|fan

POST /speed
v=0..100

## File Structure

esp_smarthome.ino
webserver.h
webserver.cpp
serial_manager.h
serial_manager.cpp
state_manager.h
state_manager.cpp
webpage.h

## Responsibilities

esp_smarthome.ino
- Boot
- WiFi
- Initialization
- Main loop

webserver.*
- Async web server
- Routes
- REST
- SSE

serial_manager.*
- UART
- Parsing
- Commands

state_manager.*
- Dashboard state
- JSON
- Sync

webpage.h
- Embedded HTML/CSS/JS

## State Structures

HomeState
- mainPower
- tube
- plug
- fan
- speed

DashboardState
- connected
- mainPower
- tube
- plug
- fan
- speed
- lastUpdate

## Web UI

Dark dashboard with:
- Main MCB
- Tube animation
- Plug animation
- Fan animation
- Fan speed slider

No login.
No device info.

## Current Status

Completed:
- UI design
- REST design
- Serial protocol
- WiFi concept
- Static IP

Pending:
- AsyncWebServer implementation
- OTA
- Arduino integration
- Multi-client testing

## Roadmap

v2.0
- Async server
- SSE
- Multi-client

v2.1
- OTA improvements

v3.0
- MQTT
- Home Assistant
- Authentication
