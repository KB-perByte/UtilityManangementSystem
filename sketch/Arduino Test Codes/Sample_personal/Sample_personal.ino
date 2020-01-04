 #include <SoftwareSerial.h>
SoftwareSerial gprsSerial(7, 8);//(tx 7,Rx 8)
#define echoPin1 2//for level detection
#define trigPin1 3

#define trigPin2 4// for counting
#define echoPin2 5
boolean locklow=true;

int t = 0, h = 0, x=0, hp = 0;//level detector
int duration, distance, RightSensor,LeftSensor;

void setup()
{
//Serial.begin (9600);
gprsSerial.begin(19200);
  Serial.begin(19200);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);

 Serial.println("Config SIM900...");
  delay(2000);
  Serial.println("Done!...");
  gprsSerial.flush();
  Serial.flush();

  // attach or detach from GPRS service 
  gprsSerial.println("AT+CGATT?");
  delay(100);
  toSerial();


  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(2000);
  toSerial();
}

void loop() {
   gprsSerial.println("AT+HTTPINIT");
   delay(2000); 
   toSerial();
 
SonarSensor(trigPin1, echoPin1);
 if((hp>1)&&(hp<103))
  {
  RightSensor = hp;
  } //ok


SonarSensor(trigPin2, echoPin2);
if((hp>=20)&&(hp<=90)) 
  {  
    if(locklow)
      {
        x++;
        LeftSensor = x;
        locklow=false;
      }
   }
  else
  {
    if(!locklow)
    {
      locklow=true;
    }
  }
  
/* Serial.print(LeftSensor);
 Serial.print(" - ");
 Serial.print(RightSensor);
 Serial.println(" %");*/
 String m = String(RightSensor);
 String s = String(LeftSensor);
   // set http param value hfgf
   Serial.println("AT+HTTPPARA=\"URL\",\"http://www.admybin.com/intern_test/1.php?sensorval1=" + s + "&sensorval2=22\"");
   Serial.println("AT+HTTPPARA=\"URL\",\"http://www.admybin.com/intern_test/1.php?sensorval1=" + s + "&sensorval2=" + m + "\"");
   // gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://www.admybin.com/intern_test/1.php?sensorval1=" + s + "&sensorval2=22\"");
   gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://www.admybin.com/intern_test/1.php?sensorval1=" + s + "&sensorval2=" + m + "\"");
   delay(2000);
   toSerial();

   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   gprsSerial.println("AT+HTTPACTION=0");
   delay(6000);
   toSerial();

   // read server response
   gprsSerial.println("AT+HTTPREAD"); 
   delay(1000);
   toSerial();

   gprsSerial.println("");
   gprsSerial.println("AT+HTTPTERM");
   toSerial();
   delay(300);

   gprsSerial.println("");
   delay(10000);

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

  void toSerial()
{
  while(gprsSerial.available()!=0)
  {
    Serial.write(gprsSerial.read());
  }
}

