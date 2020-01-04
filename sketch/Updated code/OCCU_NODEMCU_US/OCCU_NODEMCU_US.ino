 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define echoPin1 2 //for count detection
#define trigPin1 0


boolean locklow=true;

int t = 0, h = 0, x=0, hp = 0; //level detector
int duration, distance, RightSensor,LeftSensor, toiletpaper;


const char* ssid = "Energizer";
const char* password = "1qwerty2";


const char* mqtt_server = "192.168.0.55[";


WiFiClient espClient;
PubSubClient client(espClient);




const int lamp = 4;

const int lampoff = 5;


long now = millis();
long lastMeasure = 0;
long randomNumber;

void setup_wifi() {
  delay(10);
 
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
   
    if (client.connect("ESP8266Client2")) {
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


void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58;
  distance = distance - 6;  // offset correction
  distance = 50 - distance;  // height, 0 - 50 cm
  hp= 2 * distance;  // distance in %, 0-100 %
}



void setup() {
  pinMode(lamp, OUTPUT);
  
 
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
    client.connect("ESP8266Client2");
  
   
  SonarSensor(trigPin1, echoPin1);
if((hp>=20)&&(hp<=90)) 
  {  
    if(locklow)
      {
        digitalWrite(lamp, HIGH);
        digitalWrite(lampoff, LOW);
        x++;
        LeftSensor = x;
        locklow=false;
      }
   }
  else
  {
     digitalWrite(lamp, LOW);
       digitalWrite(lampoff, HIGH);
    if(!locklow)
    {
     
      locklow=true;
    }
  }

    Serial.println(LeftSensor);   //2 decimal place  
    dtostrf(LeftSensor, 6, 2, countlevel);  
    client.publish("room/occup",countlevel );
    delay(500);
  }
