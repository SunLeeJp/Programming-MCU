#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <Wire.h>

#define ssid "Hdq" //WIFI
#define password "hdq20122001"

#define mqtt_server "test.mosquitto.org"// local broker
const uint16_t mqtt_port = 1883; //MQTT Port

#define topic1 "soil_moisture"
#define D2     4  //relay

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  pinMode(A0, INPUT); //assign A0 to LDR

  setup_wifi();                             
  client.setServer(mqtt_server, mqtt_port); 
  // call the callback function to perform publish/subscribe functions
  client.setCallback(callback);
  client.subscribe("Pump"); 

  pinMode(D2, OUTPUT); // relay
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // print the connection message and IP address of ESP8266
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//execute when a device publishes to the subscribed topic.
void callback(char *topic, byte *payload, unsigned int length)
{
  //-----------------------------------------------------------------
  //print the name of the topic and the received content
  Serial.print("Notification from topic: ");
  Serial.println(topic);
  char p[length + 1];
  memcpy(p, payload, length); //convert payload from a byte array to a char array
  p[length] = NULL; //add NULL termination to the message, making it a string
  String message(p);

  if (String(topic) == "Pump"){
    if (message == "1"){
      digitalWrite(D2, HIGH);
    }
    if (message == "0"){
      digitalWrite(D2, LOW);
    }
  }   
}

//Reconnect function to reconnect when lost connection with MQTT Broker
void reconnect()
{
  while (!client.connected()) 
  {
    if (client.connect("test.mosquitto.org"))
    {
      Serial.println("Connected:"); 
      client.subscribe("Pump");
    }
    else
    {
      Serial.print("Error:, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//read sensor value
unsigned long lastMsg = 0;
void loop(){
  
  if (!client.connected()) { // Check connection
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 3000){
    lastMsg = now;
    int hum = analogRead(A0);
    hum = map(hum,0,1023,100,0);
    Serial.println("Soil Moisture: " + String(hum));
  
    //write JSON document to serial port
    StaticJsonDocument<100> doc; //allocate memory space in-place
    doc["Moisture"] = hum; // memory space for moisture
    char buffer[256]; //buffer to store the value
    serializeJson(doc, buffer); // serialize sensor memory spaces
    client.publish("soil_moisture", buffer);
    Serial.println(buffer);
  }
}
