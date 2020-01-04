int sensoroutput = 4; 

int THRESHOLD = 100;
void setup()
{
Serial.begin(9600);
}
void loop()
{
int value = analogRead(sensoroutput);  
Serial.println(value);
if (value <= THRESHOLD)                   
{
Serial.println("Water Detected");
delay(800); // to make the LED visible
}
else
Serial.println("Dry hae bhai ");
} 
