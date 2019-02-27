#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ssid "name"
#define password "0123456789"

int ledPin[] = {D1, D2, D3, D4};

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // set this for your network
  WiFi.begin(ssid, password);
  IPAddress ip(192,168,43,250);
  IPAddress subnet(255,255,255,0);
  IPAddress gateway(192,168,43,1); // my internal ip address
  WiFi.config(ip, gateway, subnet);
  
  // waiting for wifi to connect
  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    } 
    
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  // API endpoints for buttons
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
  server.handleClient();
}

void ledOn() {
    int val = server.arg("led").toInt();
        digitalWrite(ledPin[val], HIGH);
  }

void ledOff() {
    int val = server.arg("led").toInt();
        digitalWrite(ledPin[val], LOW);
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
  int val = server.arg("led").toInt();
    analogWrite(ledPin[val], intensitate);
  }

void afiseazaHTML(){
    server.send(200, "text/html", 
        "<html>\
        <head>\
            <link rel='stylesheet' href='https://cdn.jsdelivr.net/gh/titratcristina/control-leds/style.css'>\
            <script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>\
            <title>CONTROL ledULET</title>\
        </head>\
        <body>\
            <div class='container'>\
                <div class='row'>\
                    <div class='col-2'></div>\
                    <div class='col-8'>\
                        <div class='row'>\
                            <div class='col-6'>\
                                <button onclick=\"$.post('/on',{led:0})\" class='button-green'>green on</button>\
                                <button onclick=\"$.post('/on',{led:1})\" class='button-yellow'>yellow on</button>\
                                <button onclick=\"$.post('/on',{led:2})\" class='button-red'>red on</button>\
                                <button onclick=\"$.post('/on',{led:3})\" class='button-green'>green on</button>\
                                <button onclick=\"$.get('/allon')\" class='button-all'>on all</button>\
                            </div>\
                            <div class='col-6'>\
                                <button onclick=\"$.post('/off',{led:0})\" class='buttoff-green'>green off</button>\
                                <button onclick=\"$.post('/off',{led:1})\" class='buttoff-yellow'>yellow off</button>\
                                <button onclick=\"$.post('/off',{led:2})\" class='buttoff-red'>red off</button>\
                                <button onclick=\"$.post('/off',{led:3})\" class='buttoff-green'>green off</button>\
                                <button onclick=\"$.get('/alloff')\" class='buttoff-all'>off all</button>\
                            </div>\
                            <input type='number' min=0 max=4 id='ledulet'>\
                            <input type='range' min=0 max=256 id='intensitate' onchange=\"$.post('/pwm',{led:document.getElementById('ledulet').value, intensitate: document.getElementById('intensitate').value})\">\
                        </div>\
                    </div>\
                    <div class='col-2'></div>\
                </div>\
            </div>\
        </body>\
    </html>");
  }
