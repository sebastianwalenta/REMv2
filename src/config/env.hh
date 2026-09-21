#pragma once
/// CZ timezone 
#define TZ_INFO "CET-1CEST,M3.5.0/2,M10.5.0/3"

#define HOSTNAME "modelname"
//const char* hostName = "modelname";

// Display I2C pin configuration
#define DISPLAY_SDA 13
#define DISPLAY_SCL 18
#define DISPLAY_RESET U8X8_PIN_NONE

// Offline map (OpenLayers): the ESP32 has to serve ol.js/ol.css - and the map tiles -
// itself, which the firmware does not do yet, so the map stays disabled for now.
// Never link a CDN again here: the boat runs without internet access.
#define REM_ENABLE_OFFLINE_MAP 0

