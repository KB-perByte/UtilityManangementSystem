
//auth: Sagar 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define sensor A0  

const char* ssid = "Energizer";
const char* password = "1qwerty2";

const char* mqtt_server = "192.168.0.55";

WiFiClient espClient;
PubSubClient client(espClient);

long now = millis();
long lastMeasure = 0;
long randomNumber;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client1")) {
      Serial.println("connected");    
      // You can subscribe to more topics (to control more LEDs in this example)
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
   Serial.begin(115200);
   setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {
   char countlevel[7];
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client1");
  
  float volts = analogRead(sensor)*0.0048828125;  
  Serial.println("volts = "); 
  Serial.println(volts); 
  int distance = 13*pow( volts, -1); 
  delay(1000); 
  
  if (distance <= 7
  
  )
  {
    Serial.println(distance);   // print the distance
    dtostrf(distance, 6, 2, countlevel);  
    client.publish("room/soap",countlevel );
  }
 

    delay(500);
  }
