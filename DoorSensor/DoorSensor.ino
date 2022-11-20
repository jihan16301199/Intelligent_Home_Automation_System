//Author Jihan_Alam ID_16301199
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

/* Wifi Setup */
const char* ssid = "Wireless Home Network"; // replace "Wireless Home Network" with the network SSID
const char* password = "connected"; // replace "connected" with the network password

/* MQTT */
char* setState = "TASmartHome/security/DoorSensor0000"; // replace "TASmartHome/security/DoorSensor0000" with MQTT broker path
const char* server = "192.168.0.104"; // replace "192.168.0.104" with MQTT Server IP Address
const char* mqttDeviceID = "DS0000"; // replace "DS0000" with unique device ID 

long unsigned int recEventTime;         
long unsigned int interval = 100;  
boolean isClose = true;
boolean isOpen;  

int inPin = 13; //Reed switch input pin 

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
 
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect();

void setup() 
{
  pinMode(inPin, INPUT);
  digitalWrite(inPin, LOW);
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  client.begin(server, net);

  connect();

  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
}

void connect() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
  }

  while (!client.connect(mqttDeviceID)) 
  {
    delay(1000);
  }
}

void loop() 
{
  client.loop();
  delay(10);
  
  if(!client.connected()) 
  {
    connect();
  }

  httpServer.handleClient();
  
  if(digitalRead(inPin) == HIGH)
  {
    if(isClose)
    { 
      isClose = false;            
      client.publish(setState, "OPEN");  
      delay(50);
    }         
    isOpen = true;
  }

  if(digitalRead(inPin) == LOW)
  {       
    if(isOpen)
    {
      recEventTime = millis();          
      isOpen = false;       
    }
    if(!isClose && millis() - recEventTime > interval)
    {  
      isClose = true;                        
      client.publish(setState, "CLOSED");
      delay(50);
    }
  }
  
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) 
{
}
