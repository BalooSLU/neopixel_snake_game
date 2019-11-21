#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>   //Include File System Headers
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include "webserver.h"

void setup() {
  // put your setup code here, to run once:

  init_web();

  neoPixel_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
}