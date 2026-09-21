# Project Structure

```
REMv2/
├── src/
│   ├── main.cpp                  # Entry point: WiFi init, server routes, setup/loop
│   ├── actions/                  # FreeRTOS-based async effect actions
│   │   ├── Action.hh/.cpp        # Base class — runs ActionBody() in a task, supports Stop()
│   │   ├── BlinkAction.cpp
│   │   ├── LinearBlinkAction.cpp
│   │   └── RandomBlinkAction.cpp
│   ├── common/
│   │   └── DataStruct/           # Plain data structs passed between sensors and consumers
│   │       └── GyroAcceleratorDataStruct.hh
│   ├── config/
│   │   ├── env.hh                # Hostname, display pins, timezone — safe to commit
│   │   ├── secret.hh             # WiFi credentials — git-ignored, never commit
│   │   └── PinConfig.hh          # GPIO pin assignments
│   ├── display/
│   │   ├── DisplayBridge.hh      # Abstract interface: clear(), write(), drawBitmap()
│   │   ├── DisplayBridgeFactory  # Instantiates the correct DisplayBridge by enum
│   │   ├── UniversalDisplay      # Thin wrapper — holds a DisplayBridge*, delegates calls
│   │   ├── AdafruitSSD1306*      # Concrete DisplayBridge implementations
│   │   ├── ZeroFortyTwo*         # Concrete DisplayBridge implementation
│   │   └── bitmaps/              # Raw bitmap arrays for display graphics
│   ├── enums/
│   │   ├── BlockTypeEnum.hh      # controll | nav | status | action | virt | slider | inputSlider
│   │   └── DisplayTypeEnum.hh    # Display hardware variants
│   ├── events/
│   │   └── DataDrivenEvent.hh    # Helpers for sensor → display update events
│   ├── frontend/
│   │   ├── style.css             # Single source of truth for the UI styling (offline)
│   │   ├── assets/
│   │   │   └── styleCss.hh       # GENERATED from style.css - do not edit (PROGMEM string)
│   │   └── Renderer.hh/.cpp      # Generates HTML response for the / route from GroupBlock list
│   ├── modelprofiles/
│   │   ├── ModelProfile.hh       # Interface: LoadModel() → vector<GroupBlock*>
│   │   ├── ModelProvider         # Loads the active profile and exposes GetGroups()
│   │   └── R18ModelProfile       # Concrete profile for the R18 RC boat model
│   ├── objects/
│   │   ├── BasicBlock.hh/.cpp    # Base block: id, pins, name, update(), resolveInput()
│   │   ├── GroupBlock.hh/.cpp    # Container: holds blocks of a given BlockTypeEnum
│   │   ├── ControllBlocks/       # UI-driven output blocks
│   │   │   ├── OnOffBlock        # Binary GPIO toggle
│   │   │   ├── SliderBlock       # PWM/value output
│   │   │   ├── ActionBlock       # Triggers an Action (async effect)
│   │   │   └── InputSliderBlock  # Servo / ranged input
│   │   └── SensorBlocks/         # Sensor-driven display blocks
│   │       └── TextSensorBlock   # Reads from a Sensor<T> and shows value
│   ├── sensors/
│   │   ├── Sensor.hh             # Template base: ReadValue(), Begin(), onDataChanged callback
│   │   └── MPU6050Sensor         # Concrete sensor: reads gyro/accel via I2Cdevlib
│   └── service/
│       ├── StorageService        # Singleton SD card logger (appendToFile / readFile)
│       ├── GpsService            # Reads NMEA from UART, parses with TinyGPSPlus
│       ├── RealTimeService       # (Time/RTC utilities)
│       └── GpsData.hh            # GPS data struct
├── scripts/
│   └── generate_web_assets.py    # Build pre-script: src/frontend/style.css -> PROGMEM C++ header
├── include/                      # PlatformIO global includes (currently unused)
├── lib/                          # Local libraries (currently unused — deps via lib_deps)
├── test/                         # PlatformIO unit test directory
├── platformio.ini                # Build configuration
└── wokwi.toml                    # Wokwi simulator config
```

## Architecture Patterns

**Block system** — The UI and hardware are modelled as `BasicBlock` subclasses grouped into `GroupBlock` containers. `Renderer` iterates groups and blocks to build the HTML control page dynamically.

**Model profiles** — Each RC model is a `ModelProfile` subclass that constructs and returns its `GroupBlock` list. `ModelProvider` loads the active profile. To add a new model, create a new `ModelProfile` subclass and register it in `ModelProvider`.

**Display abstraction** — `DisplayBridge` is the interface; `DisplayBridgeFactory` picks the concrete implementation by `DisplayTypeEnum`. `UniversalDisplay` is the consumer-facing wrapper. To add a new display, implement `DisplayBridge` and add its enum value.

**Sensor template** — `Sensor<T>` is a template base class parameterised by a data struct (e.g. `GyroAcceleratorDataStruct`). Subclasses implement `ReadValueInternal()`. The `onDataChanged` callback decouples sensors from consumers.

**Async actions** — `Action` subclasses run their `ActionBody()` in a FreeRTOS task, allowing non-blocking LED effects. Check `StopRequested()` inside `ActionBody()` loops to support clean cancellation.

## Conventions

- Header files use `.hh` extension; implementation files use `.cpp`.
- `#pragma once` is used for all header guards.
- Config constants are `#define` macros in `env.hh`; secrets stay in `secret.hh` (git-ignored).
- Services that should have a single instance use the singleton pattern (`StorageService::getInstance()`).
- Raw pointers and `new` are used throughout (no smart pointers) — consistent with Arduino/embedded conventions.
- Avoid blocking calls in `loop()`; long-running effects belong in `Action` subclasses (FreeRTOS tasks).
- The front-end is offline: every asset is served by the ESP32 itself. Keep `https://` links out of the rendered page, embed icons as inline SVG data URIs and add new files to `scripts/generate_web_assets.py` so they end up in flash.
