#include <Arduino.h>
#include "main.h"
#include "webserv.h"

void setup()
{
  init_web();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
#ifdef TEST_WITHOUT_SNAKE_ENGINE
void left(AsyncWebServerRequest *request)
{
  Serial.println("Left");
  request->send_P(200, "text/plain", "");
}
void right(AsyncWebServerRequest *request)
{
  Serial.println("Right");
  request->send_P(200, "text/plain", "");
}
void up(AsyncWebServerRequest *request)
{
  Serial.println("Up");
  request->send_P(200, "text/plain", "");
}
void down(AsyncWebServerRequest *request)
{
  Serial.println("Down");
  request->send_P(200, "text/plain", "");
}
#endif