/*
 * Device Title: FH-SprinklerSystem
 * Device Description: MQTT Sprinkler System Control
 * Device Explanation: The MQTT server sends a message to the device and
 *                     based on the message the device turns on a relay, which
 *                     is attached to a sprinkler valve
 * 
 * 
 * Note: After flashing the code once you can remotely access your device by going to http://fh-sprinklersystem1.local/firmware
 * obviously replace HOSTNAMEOFDEVICE with whatever you defined below. The user name and password are also defined below.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>


/* WIFI Settings */
const char* ssid = "<ssid>";
const char* password = "<pass>"; 

/* Bore Hole Pump connected to pin*/
const int bhpump = 2;

/* ESP 12F Outputs */
/* Zones 1-8 */
const int channel1 = 4;
const int channel2 = 5;
const int channel3 = 9;
const int channel4 = 10;
const int channel5 = 12;
const int channel6 = 13;
const int channel7 = 14;

/* Status LED*/
const int channelReady = 1;
const int channelRunning = 3;

/* Web Updater Settings */
/* Host Name of Device */
const char* host = "FH-SprinklerSystem1";
const char* update_path = "/firmware";
const char* update_username = "<openhabid>";
const char* update_password = "<openhabpass>";

/* MQTT Settings */
// Topic which listens for commands
const char* mqttTopic = "FHSprinkler1/station/#"; 

/* MQTT Zones 1-8 Publish Topics */
const char* FHRelay1status = "FHSprinkler1/station/1/status";
const char* FHRelay2status = "FHSprinkler1/station/2/status";
const char* FHRelay3status = "FHSprinkler1/station/3/status";
const char* FHRelay4status = "FHSprinkler1/station/4/status";
const char* FHRelay5status = "FHSprinkler1/station/5/status";
const char* FHRelay6status = "FHSprinkler1/station/6/status";
const char* FHRelay7status = "FHSprinkler1/station/7/status";
const char* FHRelay8status = "FHSprinkler1/station/8/status";

/* MQTT Pump Topics */
const char* FHbhpumpstatus = "FHSprinkler1/bhpump/status";

/* MQTT Wifi Signal Publish Topics */
const char* FHRSSI = "FHSprinkler1/station/RSSI";

int* i = 0;
int lastSend = 0;
char* statusC1;
char* statusC2;
char* statusC3;
char* statusC4;
char* statusC5;
char* statusC6;
char* statusC7;
char* statusC8;
char* statusBH;
char* statusLED;

long rssi = WiFi.RSSI();
IPAddress broker(<brokerip>); 
#define CLIENT_ID "FHSprinkler1"

//webserver 
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//MQTT
WiFiClient wificlient;
PubSubClient client(wificlient);

/**
 * Check the Wifi signal strength
 */
void wifisignal() {
  long rssi = WiFi.RSSI();
  char msgBuffer[2];
  Serial.println("RSSI: ");
  Serial.println(rssi);
  client.publish(FHRSSI,dtostrf(rssi, 1, 0, msgBuffer));

  if (statusC1 == "OFF"){
     client.publish(FHRelay1status,"OFF");
  }else {
     client.publish(FHRelay1status,"ON");
  }
  if (statusC2 == "ON"){
     client.publish(FHRelay2status,"ON");
  }else {
     client.publish(FHRelay2status,"OFF");
  }
  if (statusC3 == "ON"){
     client.publish(FHRelay3status,"ON");
  }else {
     client.publish(FHRelay3status,"OFF");
  }
  if (statusC4 == "ON"){
     client.publish(FHRelay4status,"ON");
  }else {
     client.publish(FHRelay4status,"OFF");
  }
  if (statusC5 == "ON"){
     client.publish(FHRelay5status,"ON");
  }else {
     client.publish(FHRelay5status,"OFF");
  }
  if (statusC6 == "ON"){
     client.publish(FHRelay6status,"ON");
  }else {
     client.publish(FHRelay6status,"OFF");
  }
  if (statusC7 == "ON"){
     client.publish(FHRelay7status,"ON");
  }else {
     client.publish(FHRelay7status,"OFF");
  }
  if (statusBH == "ON"){
     client.publish(FHRelay8status,"OFF");
  }else {
     client.publish(FHRelay8status,"OFF");
  }     
}

/**
 * MQTT callback to process messages
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
      
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

/*    char msgString = payload; */
  if (strcmp(topic,"FHSprinkler1/station/1")==0){
     Serial.print("Entering station 1");
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel1, LOW);
      statusC1 = "ON";
      client.publish(FHRelay1status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel1, HIGH);
      statusC1 = "OFF";
      client.publish(FHRelay1status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }  
  if (strcmp(topic,"FHSprinkler1/station/2")==0){
     Serial.print("Entering station 2");
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel2, LOW);
      statusC2 = "ON";
      client.publish(FHRelay2status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel2, HIGH);
      statusC2 = "OFF";
      client.publish(FHRelay2status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }
  if (strcmp(topic,"FHSprinkler1/station/3")==0){
     Serial.print("Entering station 3");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel3, LOW);
      statusC3 = "ON";
      client.publish(FHRelay3status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel3, HIGH);
      statusC3 = "OFF";
      client.publish(FHRelay3status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }
  if (strcmp(topic,"FHSprinkler1/station/4")==0){
     Serial.print("Entering station 4");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel4, LOW);
      statusC4 = "ON"; 
      client.publish(FHRelay4status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel4, HIGH);  
      statusC4 = "OFF";
      client.publish(FHRelay4status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }
    if (strcmp(topic,"FHSprinkler1/station/5")==0){
     Serial.print("Entering station 5");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel5, LOW);
      statusC5 = "ON";
      client.publish(FHRelay5status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel5, HIGH);
      statusC5 = "OFF";
      client.publish(FHRelay5status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }
    if (strcmp(topic,"FHSprinkler1/station/6")==0){
     Serial.print("Entering station 6");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel6, LOW);
      statusC6 = "ON";
      client.publish(FHRelay6status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel6, HIGH);
      statusC6 = "OFF";
      client.publish(FHRelay6status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }

    if (strcmp(topic,"FHSprinkler1/station/7")==0){
     Serial.print("Entering station 7");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      digitalWrite(channel7, LOW);
      statusC7 = "ON";
      client.publish(FHRelay7status,"ON");
      delay(250);
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      digitalWrite(channel7, HIGH);
      statusC7 = "OFF";
      client.publish(FHRelay7status,"OFF");
    } else {
      Serial.println("Unknown value");
    }
  }
  
    if (strcmp(topic,"FHSprinkler1/station/8")==0){
     Serial.print("Entering station 8");    
  // Examine only the first character of the message
     if(payload[0] == 49)              // Message "1" in ASCII (turn outputs ON)
    {
      if (statusC8 != "ON"){
        digitalWrite(bhpump, LOW);
        statusBH = "ON";
        client.publish(FHRelay8status,"ON");
        delay(250);
      }
    } else if(payload[0] == 48)       // Message "0" in ASCII (turn outputs OFF)
    {
      if (statusC8 != "OFF"){
         digitalWrite(bhpump, HIGH);
         statusBH = "OFF";
         client.publish(FHRelay8status,"OFF");
      }
    } else {
      Serial.println("Unknown value");
    }
  }

 
  if ((statusC1 == "ON") or (statusC2 == "ON") or (statusC3 == "ON") or (statusC4 == "ON") or (statusC5 == "ON") or (statusC6 == "ON") or (statusC7 == "ON") or (statusBH == "ON")) 
  {
        digitalWrite(bhpump, LOW);
        statusC8 = "ON";
        digitalWrite(channelRunning, HIGH);
        client.publish(FHRelay8status,"ON");
        client.publish(FHbhpumpstatus,"ON");
  }  else {
        digitalWrite(bhpump, HIGH);
        statusC8 = "OFF";
        digitalWrite(channelRunning, LOW);
        client.publish(FHRelay8status,"OFF");
        client.publish(FHbhpumpstatus,"OFF");        
  }
  
  if (strcmp(topic,"FHSprinkler1/station/status")==0){
     Serial.print("Checking Sprinkler & Station Status");
  // Examine only the first character of the message
      if ((statusC1 == "ON") or (statusC2 == "ON") or (statusC3 == "ON") or (statusC4 == "ON") or (statusC5 == "ON") or (statusC6 == "ON") or (statusC7 == "ON") or (statusBH == "ON")){
        client.publish(FHbhpumpstatus,"ON");
         digitalWrite(channelRunning, HIGH);
      }else {
        client.publish(FHbhpumpstatus,"OFF");
        digitalWrite(channelRunning, LOW);
      }
      wifisignal(); 
      Serial.println("Updating Menu"); 
      delay(250); 
  }  
}

void ledsolid() {
  digitalWrite(channelReady, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
}

void ledblink() {
  digitalWrite(channelReady, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(channelReady, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID)) {
      Serial.println("connected");
      client.publish("FHSprinkler1/station/","MQTT Connected");
      wifisignal();
      ledsolid();
      client.subscribe(mqttTopic);
  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      ledblink();
      client.publish("FHSprinkler1/station/","MQTT NOT-Connected, retry in 5sec...");
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(4000);
    }
  }
}

void setup() 
{
//Reset all values to off and zero  
   
  Serial.begin(115200);
  Serial.println("Booting");
  
  // set pin modes
  pinMode(channel1, OUTPUT);
  digitalWrite(channel1, HIGH);
  statusC1 = "OFF";

  pinMode(channel2, OUTPUT);
  digitalWrite(channel2, HIGH);
  statusC2 = "OFF";
  
  pinMode(channel3, OUTPUT);
  digitalWrite(channel3, HIGH);
  statusC3 = "OFF";
  
  pinMode(channel4, OUTPUT);
  digitalWrite(channel4, HIGH);
  statusC4 = "OFF";
  
  pinMode(channel5, OUTPUT);
  digitalWrite(channel5, HIGH);
  statusC5 = "OFF";
  
  pinMode(channel6, OUTPUT);
  digitalWrite(channel6, HIGH);
  statusC6 = "OFF";
  
  pinMode(channel7, OUTPUT);
  digitalWrite(channel7, HIGH);
  statusC7 = "OFF";
    
  pinMode(bhpump, OUTPUT);
  digitalWrite(bhpump, HIGH);
  statusC8 = "OFF";
  
  pinMode(channelReady, OUTPUT);
  digitalWrite(channelReady, LOW);
  statusLED = "OFF";

  pinMode(channelRunning, OUTPUT);
  digitalWrite(channelRunning, LOW);
  statusBH = "OFF";
  
//Setup wifi, webserver and MQTT

  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("WiFi begun");

  MDNS.begin(host);
  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
   
  /* Prepare MQTT client */
  client.setServer(broker, 1883);
  client.setCallback(callback);

  Serial.println("Proceeding");
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  wifisignal();
}

void loop() 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, password);
 
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
         reconnect();
    }
  }
  
  if (client.connected()) {
      client.loop();
  }
  
  httpServer.handleClient();

  if (millis() - lastSend > 60000) {
      wifisignal();
      lastSend = millis();
  }
}
