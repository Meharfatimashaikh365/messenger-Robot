

const int trigPin = 6;  // Trigger pin of the ultrasonic sensor
const int echoPin = 7; // Echo pin of the ultrasonic sensor

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a pulse to trigger the ultrasonic sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse on the echo pin
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters using the speed of sound (343 meters/second)
  // Distance = (duration / 2) * speed of sound
  int distanceCM = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  if (distanceCM > 25)
  {
    Serial.println("The object is out of range");
  }
  else{
    Serial.print(distanceCM);
  Serial.println(" cm");

  }
  

  // Delay before the next measurement
  delay(500);
}
