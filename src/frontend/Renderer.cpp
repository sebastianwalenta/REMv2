#include "Renderer.hh"
#include <string>
#include "../objects/ControllBlocks/InputSliderBlock.hh"

void Renderer::drawBlock(GroupBlock *bl,Print &client)
{
  if(bl->type==controll || bl->type==status || bl->type==slider || bl->type==action || bl->type==inputSlider)
  {
    client.println("<div class=\"Box\">");
    client.println("<div class=\"fake-border\">");
    for(int i=0;i<bl->blocks.size();i++)
    {
        this->drawSmallBlock(bl->blocks[i],client,bl->type); 
    }
    client.println("</div>");
    client.println("</div>");
  }
  
}

void Renderer::drawSmallBlock(BasicBlock* cb,Print &client,BlockTypeEnum type)
{
  if(type==controll)
  {
    client.println("<div class=\"controll-box\">");
    std::string namee="<div class=\"textC\">";
    namee.append(cb->name);
    client.println(namee.c_str());
    std::string status= "<div class=\"status-dot\">";
    status.append(cb->actual_status);
    status.append("</div></div>");
    client.println(status.c_str());
    std::string buttons="<div class=\"button-line\">"
                        "<a href=\"/control?name=";
    buttons.append(cb->name);
    buttons.append("&state=ON\"><button class=\"on\">On</button></a>"
                   "<a href=\"/control?name=");
    buttons.append(cb->name);
    buttons.append("&state=OFF\"><button class=\"off\">Off</button></a></div>");
    client.println(buttons.c_str()); 
    //client.println("<div class=\"button-line\"><a href=\"LED_SEARCH=ON\"><button class=\"on\">On</button></a><a href=\"LED_SEARCH=OFF\"><button class=\"off\">Off</button></a></div>");
    client.println("</div>");
  }
  else if(type==action)
  {
    client.println("<div class=\"controll-box\">");
    std::string namee="<div class=\"textC\">";
    namee.append(cb->name);
    client.println(namee.c_str());
    std::string status= "<div class=\"status-dot\">";
    status.append(cb->actual_status);
    status.append("</div></div>");
    client.println(status.c_str());
    std::string buttons="<div class=\"button-line\">"
                        "<a href=\"/action?name=";
    buttons.append(cb->name);
    buttons.append("&state=ON\"><button class=\"on\">On</button></a>"
                   "<a href=\"/action?name=");
    buttons.append(cb->name);
    buttons.append("&state=OFF\"><button class=\"off\">Off</button></a></div>");
    client.println(buttons.c_str()); 
    //client.println("<div class=\"button-line\"><a href=\"LED_SEARCH=ON\"><button class=\"on\">On</button></a><a href=\"LED_SEARCH=OFF\"><button class=\"off\">Off</button></a></div>");
    client.println("</div>");
  }
  else if(type==slider)
  {
    client.println("<div class=\"controll-box\">");
    std::string namee="<div class=\"textC\">";
    namee.append(cb->name);
    client.println(namee.c_str());
    std::string status= "<div class=\"status-dot\">";
    status.append(cb->actual_status);
    status.append("%</div></div>");
    client.println(status.c_str());

    std::string slider="<div class=\"button-line\">"
                       "<input type=\"range\" min=\"0\" max=\"100\" value=\"";
    slider.append(cb->actual_status);
    slider.append("\" class=\"slider\" id=\"");
    slider.append(cb->name);
    slider.append("\" autocomplete=\"off\" oninput=\"this.parentElement.previousElementSibling.lastElementChild.innerText = this.value + '%'\" ");
    slider.append("onchange=\"fetch('/control?name=' + this.id + '&value=' + this.value)\">"
                   "</div>");
    client.println(slider.c_str());
    client.println("</div>");
  }
  else if(type==inputSlider)
  {
    InputSliderBlock* isb = static_cast<InputSliderBlock*>(cb);
    client.println("<div class=\"controll-box\">");
    std::string namee="<div class=\"textC\">";
    namee.append(cb->name);
    client.println(namee.c_str());

    std::string status= "<div class=\"status-dot\">";
    status.append(cb->actual_status);
    status.append("</div></div>");
    client.println(status.c_str());

    client.println("<div class=\"button-line\">");

    // Slider
    client.print("<input type=\"range\" min=\"0\" max=\"");
    client.print(isb->max_value);
    client.print("\" value=\"");
    client.print(cb->actual_status.c_str());
    client.print("\" class=\"slider\" style=\"width:70%\" id=\"slider_");
    client.print(cb->name);
    client.print("\" autocomplete=\"off\" oninput=\"document.getElementById('input_");
    client.print(cb->name);
    client.print("').value = this.value; this.parentElement.previousElementSibling.lastElementChild.innerText = this.value\" ");
    client.print("onchange=\"fetch('/control?name=");
    client.print(cb->name);
    client.print("&value=' + this.value)\">");

    // Input box
    client.print("<input type=\"number\" min=\"0\" max=\"");
    client.print(isb->max_value);
    client.print("\" value=\"");
    client.print(cb->actual_status.c_str());
    client.print("\" style=\"width:25%\" id=\"input_");
    client.print(cb->name);
    client.print("\" autocomplete=\"off\" oninput=\"document.getElementById('slider_");
    client.print(cb->name);
    client.print("').value = this.value; this.parentElement.previousElementSibling.lastElementChild.innerText = this.value\" ");
    client.print("onchange=\"fetch('/control?name=");
    client.print(cb->name);
    client.print("&value=' + this.value)\">");

    client.println("</div>");
    client.println("</div>");
  }
  else
  {
    cb->update();
    client.println("<div class=\"status-box\">");
    std::string namee="<div class=\"textC\" id=\"status_";
    namee.append(cb->name);
    namee.append("\">");
    namee.append(cb->name);
    client.println(namee.c_str());
    client.println("</div>");
    std::string status= "<div class=\"textC\">";
    status.append(cb->actual_status);
    status.append("</div>");
    client.println(status.c_str());
    client.println("</div>");
  }
}


void Renderer::drawHead(Print &client)
{
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset=\"UTF-8\">"); 
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  // Offline front-end: the ESP32 serves every asset itself, no CDN / no internet needed.
  // /style.css is generated from src/frontend/style.css into flash, see
  // scripts/generate_web_assets.py and the "/style.css" route in main.cpp.
  client.println("<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\">");
#if REM_ENABLE_OFFLINE_MAP
  // ol.css / ol.js are not hosted by the firmware yet - see REM_ENABLE_OFFLINE_MAP in config/env.hh
  client.println("<link rel=\"stylesheet\" href=\"/ol.css\" type=\"text/css\">");
  client.println("<script src=\"/ol.js\"></script>");
  this->drawOLMJS(client);
#endif
  client.println("<title>REM-Boat</title>");
  
  client.println("<script type=\"text/javascript\">");
  client.println("function updateStatus(name) {");
  client.println("  fetch(`/status?name=${name}`)");
  client.println("    .then(response => response.text())");
  client.println("    .then(text => {");
  client.println("      const el = document.getElementById(`status_${name}`);");
  client.println("      if (el) el.innerText = text;");
  client.println("    })");
  client.println("    .catch(err => console.error('Error updating status:', err));");
  client.println("}");

  client.println("function startStatusUpdater() {");
  client.println("  const blocks = document.querySelectorAll('[id^=\"status_\"]');");
  client.println("  blocks.forEach(b => {");
  client.println("    const name = b.id.replace('status_', '');");
  client.println("    updateStatus(name);");
  client.println("  });");
  client.println("}");

  client.println("setInterval(startStatusUpdater, 3000);");
  client.println("</script>");
  client.println("</head>");
}

void Renderer::drawHeader(Print &client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  this->drawHead(client);
  client.println("<body>");
  client.println("<h1>REM-Boat</h1>");
  client.println("<div class=\"container\">");
}

// OpenLayers map - rendered only when REM_ENABLE_OFFLINE_MAP is enabled (config/env.hh).
// Everything it loads has to come from the ESP32 itself: /ol.js, /ol.css and the tiles.
#if REM_ENABLE_OFFLINE_MAP
void Renderer::drawOLMJS(Print &client)
{
  client.println("<script type=\"text/javascript\">");
  // Offline tiles served by the ESP32 (data/tiles/{z}/{x}/{y}.png + "pio run -t uploadfs")
  client.println("function start(){var map = new ol.Map({target: \"map\",layers: [new ol.layer.Tile({source: new ol.source.XYZ({url: '/tiles/{z}/{x}/{y}.png'})})],view: new ol.View({center: ol.proj.fromLonLat([18.610968,49.754749]),zoom:17 })});");
  client.println("var markers = new ol.layer.Vector({");
  // Inline marker (SVG data URI) - no external icon file is loaded.
  client.println("source: new ol.source.Vector(),style: new ol.style.Style({image: new ol.style.Icon({anchor: [0.5, 1],src: 'data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIzNiIgdmlld0JveD0iMCAwIDI0IDM2Ij48cGF0aCBkPSJNMTIgMEM1LjQgMCAwIDUuNCAwIDEyYzAgOSAxMiAyNCAxMiAyNHMxMi0xNSAxMi0yNGMwLTYuNi01LjQtMTItMTItMTJ6IiBmaWxsPSIjZjQ0MzM2Ii8+PGNpcmNsZSBjeD0iMTIiIGN5PSIxMiIgcj0iNSIgZmlsbD0iI2ZmZmZmZiIvPjwvc3ZnPg=='})})});");
  client.println("map.addLayer(markers);");

 client.println("var marker = new ol.Feature(new ol.geom.Point(ol.proj.fromLonLat([18.610968, 49.754749])));");
 client.println("markers.getSource().addFeature(marker);");
  client.println("}</script>");
}
#endif


void Renderer::drawNew(std::vector<GroupBlock*>blocks,Print &client)
{   
    ///////////////////////////head////////////////
    this->drawHead(client);
    //////////////////////body////////////////////
#if REM_ENABLE_OFFLINE_MAP
    client.println("<body onload=start()>");
#else
    client.println("<body>");
#endif
    client.println("<h1>REM</h1>");
    client.println("<h3>RemoteEffectManager</h3>");
#if REM_ENABLE_OFFLINE_MAP
    client.println("<div id=\"map\" class=\"map\"></div>");
#endif
    client.println("<div class=\"container\">");
    for(int i=0;i<blocks.size();i++)
    {
        this->drawBlock(blocks[i],client);
    }
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");
}
