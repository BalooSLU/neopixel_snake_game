#ifndef MAIN_SNAKE_H
#define MAIN_SNAKE_H

#define TEST_WITHOUT_SNAKE_ENGINE
#include <ESPAsyncWebServer.h>

extern void left(AsyncWebServerRequest *request);
extern void right(AsyncWebServerRequest *request);
extern void up(AsyncWebServerRequest *request);
extern void down(AsyncWebServerRequest *request);

#endif