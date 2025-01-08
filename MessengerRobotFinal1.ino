#include "U8glib.h"          // Library for OLED display
#include <TinyGPS++.h>       // Library for GPS
#include <math.h>            // For distance calculations

// OLED display initialization
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
TinyGPSPlus gps;

// Constants for ultrasonic and IR sensors
const int trigPin = 6;
const int echoPin = 7;
const int IRSensor = 2;

// GPS tolerance to handle deviations
const float TOLERANCE = 0.00001;

// Room coordinates (latitude, longitude, and room name)
struct Room {
  float latitude;
  float longitude;
  String name;
};

Room rooms[] = {
  {67.500000, 25.000000, "Room 0"},
  {67.500000, 24.999850, "Room 1"},
  {67.499850, 25.000000, "Room 2"},
  {67.499850, 24.999850, "Room 3"}
};

const int numRooms = sizeof(rooms) / sizeof(rooms[0]);

// Timing intervals
unsigned long gpsUpdateInterval = 1000;   // Update GPS every 1000 ms
unsigned long ultrasonicInterval = 500;  // Update ultrasonic every 500 ms
unsigned long IRInterval = 1000;         // Update IR every 1000 ms

unsigned long lastGpsUpdate = 0;
unsigned long lastUltrasonicUpdate = 0;
unsigned long lastIRUpdate = 0;

// Setup for ultrasonic sensor
void ultrasonicSetup() {
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(8, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Setup for IR sensor
void IRSetup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
}

// Function to calculate Euclidean distance
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  float dLat = lat1 - lat2;
  float dLon = lon1 - lon2;
  return sqrt(dLat * dLat + dLon * dLon);
}

// KNN-style room classification with tolerance for deviations
String classifyRoomWithTolerance(float lat, float lon) {
  for (int i = 0; i < numRooms; i++) {
    if (abs(lat - rooms[i].latitude) <= TOLERANCE && abs(lon - rooms[i].longitude) <= TOLERANCE) {
      return rooms[i].name;
    }
  }
  return "Unknown";
}

// Function to display location and room information on OLED
void displayLocationOnOLED(float latitude, float longitude, String room) {
  u8g.setFont(u8g_font_unifont);

  u8g.drawStr(2, 10, "Lat: ");
  char latStr[15];
  dtostrf(latitude, 11, 7, latStr);
  u8g.drawStr(40, 10, latStr);

  u8g.drawStr(2, 25, "Long: ");
  char longStr[15];
  dtostrf(longitude, 11, 7, longStr);
  u8g.drawStr(40, 25, longStr);

  u8g.drawStr(2, 40, "Room: ");
  u8g.drawStr(50, 40, room.c_str());
}

// Function to display sensor data and room classification
void displayInfo() {
  // IR Sensor
  Serial.print("IR Sensor: ");
  int sensorValue = digitalRead(IRSensor);
  if (sensorValue == LOW) {
    Serial.println("Object Detected!");
  } else {
    Serial.println("No Object Detected.");
  }

  delay(10);

  // Ultrasonic Sensor
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distanceCM = duration * 0.034 / 2;
  Serial.print("Ultrasonic Distance: ");
  if (distanceCM > 25) {
    Serial.println("Out of Range");
  } else {
    Serial.print(distanceCM);
    Serial.println(" cm");
  }

  delay(10);

  // GPS and Room Classification
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  String room = classifyRoomWithTolerance(latitude, longitude);

  Serial.print("Lat: ");
  Serial.println(latitude, 7);
  Serial.print("Long: ");
  Serial.println(longitude, 7);
  Serial.print("Room: ");
  Serial.println(room);

  // Display on OLED
  u8g.firstPage();
  do {
    displayLocationOnOLED(latitude, longitude, room);
  } while (u8g.nextPage());

  Serial.println();  // Add a couple of new lines for clarity
}

// Setup function
void setup() {
  ultrasonicSetup();
  IRSetup();

  Serial.begin(9600);
  Serial3.begin(9600); // For GPS module

  u8g.begin();
}

// Main loop function
void loop() {
  while (Serial3.available() > 0) {
    char c = Serial3.read();
    if (gps.encode(c)) {
      displayInfo();
    }
  }

  unsigned long currentMillis = millis();

  // Update ultrasonic sensor at intervals
  if (currentMillis - lastUltrasonicUpdate >= ultrasonicInterval) {
    lastUltrasonicUpdate = currentMillis;
    long duration;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    int distanceCM = duration * 0.034 / 2;
    Serial.print("Updated Ultrasonic Distance: ");
    if (distanceCM > 25) {
      Serial.println("Out of Range");
    } else {
      Serial.print(distanceCM);
      Serial.println(" cm");
    }
  }

  // Update IR sensor at intervals
  if (currentMillis - lastIRUpdate >= IRInterval) {
    lastIRUpdate = currentMillis;
    int sensorValue = digitalRead(IRSensor);
    Serial.print("Updated IR Sensor: ");
    if (sensorValue == LOW) {
      Serial.println("Object Detected!");
    } else {
      Serial.println("No Object Detected.");
    }
  }
}
