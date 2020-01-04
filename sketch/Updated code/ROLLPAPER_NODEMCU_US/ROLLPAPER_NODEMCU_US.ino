 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define echoPin1 2 //for level detection of toliet roll
#define trigPin1 0

int t = 0, h = 0, x=0, hp = 0; //level detector
int duration, distance, RightSensor,LeftSensor, toiletpaper;

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
    // Attempt to connect
    if (client.connect("ESP8266Client3")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

 
/****************** SonarSensor ****************************************
Author:  Sagar Paul
Input:   Trigger and Echo state 
Output:  the calculated sensor gives distance 
Remarks: distance - x will correct the offset, y -  distance will give data based on the height of 
          the material we are using to measure. ultimately giving data as HP that is the distance from 
         the object to the sensor -  offset. 
************************************************************************************/ 
void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58;
  distance = distance - 2;  // offset correction
  distance = 15 - distance;  // height, 0 - 50 cm
  hp= 2 * distance;  // distance in %, 0-100 %
}



void setup() {
 
  Serial.begin(115200);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
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
    client.connect("ESP8266Client3");
  
   
  SonarSensor(trigPin1, echoPin1);
   if((hp>1)&&(hp<103))
    {
    RightSensor = hp;
    } //ok

    Serial.println(RightSensor);   //2 decimal place  
    dtostrf(RightSensor, 6, 2, countlevel);  
    client.publish("room/paper",countlevel );
    delay(500);
  }
