# REM V2
- Second version of Remote Effect Manager. Simple ESP32 based program for controling effect and colecting data from RC models.

## Offline front-end

The model runs without internet access, so the ESP32 serves the whole UI itself - no CDN,
no web fonts, no remote images.

| Route | Served | Source |
|---|---|---|
| `/` | control page (HTML) | `Renderer::drawNew()` |
| `/style.css` | stylesheet streamed from flash | `src/frontend/style.css` |

`src/frontend/style.css` is the single source of truth for the styling. The build converts it
into a PROGMEM string in `src/frontend/assets/styleCss.hh` using the PlatformIO pre-script
`scripts/generate_web_assets.py` (manual run: `python scripts/generate_web_assets.py`). Edit the
`.css` file and build - the generated header is rewritten only when the CSS actually changed.

Rules for anything added to the page:

- never link `https://...` (CDN, fonts, tiles) - the model is offline
- embed small images (e.g. map markers) as inline SVG data URIs
- serve bigger assets from flash (PROGMEM) or from the file system

The OpenLayers map is switched off via `REM_ENABLE_OFFLINE_MAP` in `src/config/env.hh`:
`ol.js`/`ol.css` are not hosted by the firmware yet. The page, the marker and the tile URL
template are already offline-ready, so enabling it only needs the library (e.g. gzipped in
flash or LittleFS) plus local tiles at `/tiles/{z}/{x}/{y}.png`.

# TODO
- name for groups
- global action for group (for type)
- reporting RSSI
- react to event ARDUINO_EVENT_WIFI_STA_DISCONECTED (wifi.onevent)
- config HOSTNAME (wifi.setHostname())
- offline map: host ol.js/ol.css on the device + local tiles for the boating area
