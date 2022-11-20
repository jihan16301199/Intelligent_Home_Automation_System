//Author Jihan_Alam ID_16301199
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

/* Wifi Setup */
const char* ssid = "Wireless Home Network"; // replace "Wireless Home Network" with the network SSID
const char* password = "connected"; // replace "connected" with the network password

/* MQTT Settings */
char* getState = "TASmartHome/utilities/SwitchSystem0001"; // replace "TASmartHome/utilities/SwitchSystem0001" with MQTT broker path
const char* server = "192.168.0.104"; // replace "192.168.0.104" with MQTT Server IP Address
const char* mqttDeviceID = "SS0001"; // replace "SS0000" with unique device ID 

int outPin1 = 14; //Relay output pin 1
int outPin2 = 12; //Relay output pin 2
int outPin3 = 13; //Relay output pin 3
int outPin4 = 16; //Relay output pin 4

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect();

void setup() 
{
  pinMode(outPin1, OUTPUT);
  digitalWrite(outPin1, LOW);

  pinMode(outPin2, OUTPUT);
  digitalWrite(outPin2, LOW);

  pinMode(outPin3, OUTPUT);
  digitalWrite(outPin3, LOW);

  pinMode(outPin4, OUTPUT);
  digitalWrite(outPin4, LOW);

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
  client.subscribe(getState);
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
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) 
{
    String storeUpdate = payload;

  if (storeUpdate == "S1ON")
  {
    digitalWrite(outPin1, HIGH);
    delay(250);
  }
  else if (storeUpdate == "S1OFF")
  {
    digitalWrite(outPin1, LOW);
    delay(250);
  }
  else if (storeUpdate == "S2ON")
  {
    digitalWrite(outPin2, HIGH);
    delay(250);
  }
  else if (storeUpdate == "S2OFF")
  {
    digitalWrite(outPin2, LOW);
    delay(250);
  }
  else if (storeUpdate == "S3ON")
  {
    digitalWrite(outPin3, HIGH);
    delay(250);
  }
  else if (storeUpdate == "S3OFF")
  {
    digitalWrite(outPin3, LOW);
    delay(250);
  }
  else if (storeUpdate == "S4ON")
  {
    digitalWrite(outPin4, HIGH);
    delay(250);
  }
  else if (storeUpdate == "S4OFF")
  {
    digitalWrite(outPin4, LOW);
    delay(250);
  }
}
