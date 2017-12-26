#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EasyDDNS.h>

#define ssid "....."
#define password "....."

int ledPin[] = {D1, D2, D3, D4};

ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  IPAddress ip(192,168,1,200);
  IPAddress subnet(255,255,255,0);
  IPAddress gateway(192,168,1,1);
  WiFi.config(ip, gateway, subnet);
  
  
  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    } 
    
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();

  EasyDDNS.service("noip");
  EasyDDNS.client("hostname","username","password");

  server.on("/on", ledOn);
  server.on("/off", ledOff);
  server.on("/", afiseazaHTML);

  for(int i=0; i<4; i++){
      pinMode(ledPin[i], OUTPUT);
    }

}

void loop() {
  server.handleClient();
  EasyDDNS.update(10000);
}

void ledOn() {
    for(int i=0; i<4; i++){
        digitalWrite(ledPin[i], HIGH);
      }
  }

void ledOff() {
    for(int i=0; i<4; i++){
        digitalWrite(ledPin[i], LOW);
      }
  }

void afiseazaHTML(){
    server.send(200, "text/html", 
      "<html>\
      <head>\
          <script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"\ integrity=\"sha256hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4=\"\ crossorigin=\"anonymous\"></script>\
          <title>CONTROL BECULETE</title>\
          <style>\
           button{\
            border: none;\
            color: white;\
            padding: 100px;\
            text-align: center;\
            display: inline-block;\
            font-size: 50px;\
            border-radius: 12px;\
            width: 100%;\
            margin-bottom: 20px;\
            margin-top: 10px;\
           }\
           button.off{\
            background-color: #DD1C1A;\
           }\
           button.on{\
            background-color: #4CAF50;\
          }\
           button:focus{\
            background-color: grey;\
           }\
         </style>\
        </head>\
        <body>\
          <button onclick=\"$.get('/on')\">On</button>\
          <button onclick=\"$.get('/off')\">Off</button>\
        </body>\
       <html>");
  }

