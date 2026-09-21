#include <vector>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <ESPmDNS.h>
#include <ElegantOTA.h>

#include "config/secret.hh"
#include "config/env.hh"
#include <U8g2lib.h>
#include "objects/GroupBlock.hh"
#include "objects/BasicBlock.hh"
#include "frontend/Renderer.hh"
#include "frontend/assets/styleCss.hh"
#include "enums/BlockTypeEnum.hh"
#include "objects/ControllBlocks/OnOffBlock.hh"
#include "objects/ControllBlocks/SliderBlock.hh"
#include "objects/ControllBlocks/ActionBlock.hh"
#include "objects/ControllBlocks/InputSliderBlock.hh"
#include "objects/SensorBlocks/TextSensorBlock.hh"
#include "sensors/Sensor.hh"
#include "sensors/MPU6050Sensor.hh"
#include "service/StorageService.hh"
#include "service/GpsService.hh"

#include "display/bitmaps/boatBitmap.hh"

#include "display/UniversalDisplay.hh" 
#include "enums/DisplayTypeEnum.hh"

#include "events/DataDrivenEvent.hh"
#include "common/DataStruct/GyroAcceleratorDataStruct.hh"
#include "modelprofiles/modelprovider.hh"

AsyncWebServer server(80);


std::vector<Sensor<GyroAcceleratorDataStruct>*> Sensors;
UniversalDisplay* display;
GpsService* gpsService;
std::vector<GroupBlock*> Groups;
ModelProvider* modelProvider;

Renderer *rd;

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  delay(10);

 /*StorageService* storage = StorageService::getInstance(5);

  if (!storage) {
      Serial.println("StorageService cant be inicialized");
      return;
  }*/
/*
  bool success = storage->appendToFile("data", "Test data from REMv2");
  if (success) {
      Serial.println("Testdata writen");
  } else {
      Serial.println("error while testing data");
  }

  std::string content = storage->readFile("data");
    if (!content.empty()) {
        Serial.println("file content:");
        Serial.println(content.c_str());
    }
        */
  
  pinMode(2, OUTPUT);
  delay(10);

  //Wire.begin(DISPLAY_SDA, DISPLAY_SCL);
  delay(200);

  rd=new Renderer();
  /*gpsService=new GpsService(27,26);
  gpsService->begin();*/

  display = new UniversalDisplay(
      DisplayTypeEnum::AdafruitSSD1306_64X32,
      DISPLAY_SDA,
      DISPLAY_SCL,
      DISPLAY_RESET
  );
  display->write("REMv2");


  modelProvider = new ModelProvider();
  Groups = modelProvider->GetGroups();

  /*
  Groups.push_back(new GroupBlock(2,status));
  Sensor<GyroAcceleratorDataStruct>* movementSensor=new MPU6050Sensor("Movement",1000,33,32);
  Sensors.push_back(movementSensor);

  movementSensor->SetOnDataChanged([&](std::string angle){
      DataDrivenEvent::OnDataChanged(display, angle);
  });
  */

  /*Groups.back()->blocks.push_back(new TextSensorBlock<GyroAcceleratorDataStruct>(2, 0, {}, "Movement",movementSensor));
  Groups.push_back(new GroupBlock(3,action));
  Groups.back()->blocks.push_back(new ActionBlock(3, 0, {15},"Action"));*/

  /*Groups.push_back(new GroupBlock(4,inputSlider));
  Groups.back()->blocks.push_back(new InputSliderBlock(4, 0, {17},"Servo", 180));*/

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  display->write(WiFi.localIP().toString().c_str());

  if(!MDNS.begin(HOSTNAME))
  {
    Serial.println("mDNS init Fail");
  }
  else
  {
      Serial.println("");
      Serial.println("mDNS started");
      Serial.print("URL to connect: ");
      Serial.print("http://");
      Serial.print(HOSTNAME);
      Serial.print(".local");
      Serial.println("/");
  }

  ElegantOTA.begin(&server);
  Serial.println("OTA Ready on /update");

  
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request) 
  {
    if (request->hasParam("name")) {
      String name  = request->getParam("name")->value();

      if (request->hasParam("state")) {
        String state = request->getParam("state")->value();
        Serial.print("Control: name="); Serial.print(name); Serial.print(", state="); Serial.println(state);

        for (auto group : Groups) {
          for (auto block : group->blocks) {
            if (group->type == controll) {
              OnOffBlock* onoff = static_cast<OnOffBlock*>(block);
              if (onoff && name.equalsIgnoreCase(onoff->name)) {
                if (state.equalsIgnoreCase("ON")) {
                  onoff->setPin(true);
                } else if (state.equalsIgnoreCase("OFF")) {
                  onoff->setPin(false);
                }
              }
            }
          }
        }
      } else if (request->hasParam("value")) {
        int value = request->getParam("value")->value().toInt();
        Serial.print("Control: name="); Serial.print(name); Serial.print(", value="); Serial.println(value);
        for (auto group : Groups) {
          if (group->type == slider) {
            for (auto block : group->blocks) {
              if (name.equalsIgnoreCase(block->name)) {
                SliderBlock* sb = static_cast<SliderBlock*>(block);
                if (sb) sb->setValue(value);
              }
            }
          } else if (group->type == inputSlider) {
            for (auto block : group->blocks) {
              if (name.equalsIgnoreCase(block->name)) {
                InputSliderBlock* isb = static_cast<InputSliderBlock*>(block);
                if (isb) isb->setValue(value);
              }
            }
          }
        }
      }
      request->redirect("/");
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if (!request->hasParam("name") || !request->hasParam("state")) {
      request->send(400, "text/plain", "Missing parameters");
      return;
    }

    String name = request->getParam("name")->value();
    String state = request->getParam("state")->value();
    Serial.print("Action: name="); Serial.print(name); Serial.print(", state="); Serial.println(state);

    for (auto group : Groups) {
      if (group->type != action) {
        continue;
      }

      for (auto block : group->blocks) {
        if (!name.equalsIgnoreCase(block->name)) {
          continue;
        }

        ActionBlock* actionBlock = static_cast<ActionBlock*>(block);
        if (actionBlock != nullptr) {
          if (state.equalsIgnoreCase("ON")) {
            actionBlock->setPin(true);
          } else if (state.equalsIgnoreCase("OFF")) {
            actionBlock->setPin(false);
          }
        }
      }
    }

    request->redirect("/");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) 
  {
    if (!request->hasParam("name")) {
      request->send(400, "text/plain", "Missing parameter: name");
      return;
    }

    String name = request->getParam("name")->value();
    bool found = false;

    for (auto sensor : Sensors) {
      if (sensor && name.equalsIgnoreCase(sensor->name.c_str())) {
        request->send(200, "text/plain", sensor->lastValue.c_str());
        found = true;
        break;
      }
    }

    if (!found) {
      request->send(404, "text/plain", "Sensor not found");
    }
  });

  // Offline front-end asset: the stylesheet lives in flash, generated from
  // src/frontend/style.css by scripts/generate_web_assets.py (no CDN, no internet).
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", (const uint8_t *)STYLE_CSS, sizeof(STYLE_CSS) - 1);
    response->addHeader("Cache-Control", "public, max-age=86400");
    request->send(response);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    rd->drawNew(Groups, *response);   
    request->send(response);
  });

  for (auto sensor: Sensors) {
    sensor->Begin();
  }
  
  server.begin();
  Serial.println("Async server started");



}

void loop() 
{
  for (auto sensor: Sensors) 
  {
    sensor->ReadValue();
    
  }
  ElegantOTA.loop();
  delay(10);
}
