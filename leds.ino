#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EasyDDNS.h>

#define ssid "........"
#define password "........."

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
  EasyDDNS.client();

  server.on("/on", ledOn);
  server.on("/off", ledOff);
  server.on("/allon", ledsOn);
  server.on("/alloff", ledsOff);
  server.on("/pwm", pwmLed);
  server.on("/", afiseazaHTML);

  for(int i=0; i<4; i++){
      pinMode(ledPin[i], OUTPUT);
    }

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  EasyDDNS.update(10000);
}

void ledOn() {
    int valoare = server.arg("led").toInt();
        digitalWrite(ledPin[valoare], HIGH);
  }

void ledOff() {
    int valoare = server.arg("bec").toInt();
        digitalWrite(ledPin[valoare], LOW);
  }

void ledsOn() {
    for(int i=0; i<4; i++){
        digitalWrite(ledPin[i], HIGH);
      }
  }

void ledsOff() {
    for(int i=0; i<4; i++){
        digitalWrite(ledPin[i], LOW);
      }
  }

void pwmLed() {
  int intensitate = server.arg("intensitate").toInt();
  int valoare = server.arg("led").toInt();
    analogWrite(ledPin[valoare], intensitate);
  }

void afiseazaHTML(){
    server.send(200, "text/html", 
      "<html>\
        <head>\
          <script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>\
          <title>CONTROL BECULET</title>\
        </head>\
        <body>\
            <button onclick=\"$.post('/on',{led:0})\">green on</button>\
            <button onclick=\"$.post('/on',{led:1})\">yellow on</button>\
            <button onclick=\"$.post('/on',{led:2})\">red on</button>\
            <button onclick=\"$.post('/on',{led:3})\">green on</button>\
            <button onclick=\"$.get(/onall)\">off all</button>\
            <button onclick=\"$.post('/off',{bec:0})\">green off</button>\
            <button onclick=\"$.post('/off',{bec:1})\">yellow off</button>\
            <button onclick=\"$.post('/off',{bec:2})\">red off</button>\
            <button onclick=\"$.post('/off',{bec:3})\">green off</button>\
            <button onclick=\"$.get(/offall)\">off all</button>\
            <input type=\"number\" min=0 max=4 id=\"beculet\">\
            <input type=\"range\" min=0 max=256 id=\"intensitate\" onchange=\"$.post('/pwm',{led:document.getElementById('beculet').value, intensitate: document.getElementById('intensitate').value})\">\    
        </body>\
       <\html>");
  }
