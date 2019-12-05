#include "webserv.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

extern void left(AsyncWebServerRequest *request);
extern void right(AsyncWebServerRequest *request);
extern void up(AsyncWebServerRequest *request);
extern void down(AsyncWebServerRequest *request);

const char* ssid = "Snake_HAW";
const char* password = "haw.1234";
const char index_html[] ="<!DOCTYPE html> <html> <head> <meta charset=\"utf-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\" /> <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\"> </head> <body> <div class=\"container-fluid\"> <div class=\"col-xs-12\" style=\"height: 100vh\"> <div class=\"row\" style=\"height: 33.33%; padding-top: 1em; padding-bottom:1em\"> <div class=\"col-xs-8\" ></div> <div class=\"col-xs-4\" style=\"text-align: center; height: 100%\"> <button id=\"up\" type=\"button\" class=\"btn btn-default\" style=\"height: 100%; width: 100%\" onmousedown='makeAjaxCall(\"up\")' onmouseup='makeAjaxCall(\"driveStop\")' ontouchstart='makeAjaxCall(\"up\")' ontouchend='makeAjaxCall(\"driveStop\")'>Up</button> </div> </div> <div class=\"row\" style=\"height: 33.33%; padding-bottom:1em\"> <div class=\"col-xs-4\" style=\"height: 100%; text-align: center\"> <button id=\"left\" type=\"button\" class=\"btn btn-default\" style=\"height: 100%; width: 100%\" onmousedown='makeAjaxCall(\"left\")' onmouseup='makeAjaxCall(\"steerStop\")' ontouchstart='makeAjaxCall(\"left\")' ontouchend='makeAjaxCall(\"steerStop\")'>Left</button> </div> <div class=\"col-xs-4\" style=\"height: 100%; text-align: center\"> <button id=\"right\" type=\"button\" class=\"btn btn-default\" style=\"height: 100%; width: 100%\" onmousedown='makeAjaxCall(\"right\")' onmouseup='makeAjaxCall(\"steerStop\")' ontouchstart='makeAjaxCall(\"right\")' ontouchend='makeAjaxCall(\"steerStop\")'>Right</button> </div> <div class=\"col-xs-4\" ></div> </div> <div class=\"row\" style=\"height: 33.33%; padding-bottom:1em\"> <div class=\"col-xs-8\" ></div> <div class=\"col-xs-4\" style=\"height: 100%; text-align: center\"> <button id=\"down\" type=\"button\" class=\"btn btn-default\" style=\"height: 100%; width: 100%\" onmousedown='makeAjaxCall(\"down\")' onmouseup='makeAjaxCall(\"driveStop\")' ontouchstart='makeAjaxCall(\"down\")' ontouchend='makeAjaxCall(\"driveStop\")'>Down</button> </div> </div> </div> </div> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script> <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script> <script> function makeAjaxCall(url){$.ajax({\"url\": url})}</script> <!--<script> document.addEventListener('keydown', function(event) { if(event.keyCode == 37) { //Left Arrow makeAjaxCall(\"left\"); } else if(event.keyCode == 39) { //Right Arrow makeAjaxCall(\"right\"); } else if(event.keyCode == 38) { //Up Arrow makeAjaxCall(\"up\"); } else if(event.keyCode == 40) { //Down Arrow makeAjaxCall(\"down\"); } }); document.addEventListener('keyup', function(event) { if(event.keyCode == 37 ||event.keyCode == 39 ) { //Left or Right Arrow makeAjaxCall(\"steerStop\"); } else if(event.keyCode == 38 ||event.keyCode == 40 ) { //Up or Down Arrow makeAjaxCall(\"driveStop\"); } }); </script>--> </body> </html>";

void notFound(AsyncWebServerRequest *request) {
    request->send_P(404, "text/plain", "Not found");
}

void init_web(void)
{
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html",index_html);
        }
    );

   // Handle a POST request
    server.on("/left", left);
    server.on("/right", right);
    server.on("/up", up);
    server.on("/down", down);
    server.onNotFound(notFound);
    server.begin();

}


void left(AsyncWebServerRequest *request){
    request->send_P(200, "text/plain","");      // sending a success massage

}
void right(AsyncWebServerRequest *request){
    request->send_P(200, "text/plain","");      // sending a success massage

}
void up(AsyncWebServerRequest *request){
    request->send_P(200, "text/plain","");      // sending a success massage

}
void down(AsyncWebServerRequest *request){
    request->send_P(200, "text/plain","");      // sending a success massage

}