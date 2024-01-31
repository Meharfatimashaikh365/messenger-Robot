const int IRSensor = 2; // Connect the digital output pin to Arduino digital pin 2

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(IRSensor); // Read digital value from IR sensor

  if (sensorValue == LOW) {
    Serial.println("Object Detected!");
  } else {
    Serial.println("No Object Detected.");
  }

  delay(500); // Add a delay for stability
}
