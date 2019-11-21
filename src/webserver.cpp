#include <ESP8266WiFi.h>
#include <FS.h>   //Include File System Headers
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include "webserver.h"

AsyncWebServer server(80);

const char* host = "Snake";
const char* password = "haw.1";


void notFound(AsyncWebServerRequest *request) {
    request->send_P(404, "text/plain", "Not found");
}

void init_web(void)
{
    WiFi.softAP(host, password);
    softap_config config;
    wifi_softap_get_config(&config);
    config.max_connection = 1;
    
    wifi_softap_set_config(&config);
    WiFi.softAPIP();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS,"/index.html","text/html");
    });
    server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request)
   {
    request->send(SPIFFS, "/main.css","text/css");
   }); 
   
   server.on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request)
   {
    request->send(SPIFFS,"/jquery.js", "application/javascript");
   }); 
   server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
   {
    request->send(SPIFFS,"/script.js","application/javascript");
   });
   server.on("/colorwheel1.png", HTTP_GET, [](AsyncWebServerRequest *request)
   {
    request->send(SPIFFS,"/colorwheel1.png","image/png");
   });

   // Handle a POST request
    /*server.on("/post", handleRgbData);

    server.on("/off", handleOff);
    server.on("/auto", handleAuto);
    */
   
    server.onNotFound(notFound);
    MDNS.begin(host); 
    server.begin();
    // Add service to MDNS-SD
   MDNS.addService("http", "tcp", 80);
}