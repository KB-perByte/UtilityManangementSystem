


int smokeA0 = A5;
// Your threshold value
int sensorThres = 400;

void setup() {
  
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    Serial.println("Detected");
    //digitalWrite(redLed, HIGH);
    //digitalWrite(greenLed, LOW);
    //tone(buzzer, 1000, 200);
  }
  else
  {
    Serial.println("Good Condition");
    //digitalWrite(redLed, LOW);
    //digitalWrite(greenLed, HIGH);
    //noTone(buzzer);
  }
  delay(100);
}
