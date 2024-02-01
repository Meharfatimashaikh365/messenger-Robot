#include "U8glib.h"
#include <TinyGPS++.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
TinyGPSPlus gps;

const int trigPin = 6;    // Trigger pin of the ultrasonic sensor
const int echoPin = 7;    // Echo pin of the ultrasonic sensor
const int IRSensor = 2;   // Connect the digital output pin to Arduino digital pin 2

unsigned long gpsUpdateInterval = 1000;  // Update GPS every 1000 milliseconds
unsigned long ultrasonicInterval = 500;  // Update ultrasonic every 500 milliseconds
unsigned long IRInterval = 1000;         // Update IR every 1000 milliseconds

unsigned long lastGpsUpdate = 0;
unsigned long lastUltrasonicUpdate = 0;
unsigned long lastIRUpdate = 0;

void ultrasonicSetup() {
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(8, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void IRSetup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
}

void u8g_prepare(void) {
  u8g.setFont(u8g_font_4x6);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void displayLocationOnOLED() {
  if (gps.location.isValid()) {
    u8g.setFont(u8g_font_unifont);

    u8g.drawStr(2, 10, "Lat: ");
    char latitude[15];
    dtostrf(gps.location.lat(), 11, 7, latitude);
    u8g.drawStr(40, 10, latitude);

    u8g.drawStr(2, 25, "Long: ");
    char longitude[15];
    dtostrf(gps.location.lng(), 11, 7, longitude);
    u8g.drawStr(40, 25, longitude);
  } else {
    u8g.setFont(u8g_font_4x6);
    u8g.drawStr(2, 10, "Location: N/A");
  }
}

void draw(void) {
  u8g_prepare();
  displayLocationOnOLED();
}

void displayInfo() {
  Serial.print("IR Sensor: ");
  int sensorValue = digitalRead(IRSensor);
  if (sensorValue == LOW) {
    Serial.println("Object Detected!");
  } else {
    Serial.println("No Object Detected.");
  }

  delay(10);  // Add a small delay for better readability on the Serial Monitor

  // Update Ultrasonic
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
    Serial.println("The object is out of range");
  } else {
    Serial.print(distanceCM);
    Serial.println(" cm");
  }

  delay(10);  // Add a small delay for better readability on the Serial Monitor

  // Update GPS
  Serial.print("Lat: ");
  Serial.println(gps.location.lat(), 7);
  Serial.print("Long: ");
  Serial.println(gps.location.lng(), 7);

  Serial.println();  // Add a couple of new lines for separation

  lastGpsUpdate = millis();
}

void ultrasonicLoop() {
  // Not necessary to include ultrasonicLoop() here as it's already called in the loop function
}

void IRLoop() {
  // Not necessary to include IRLoop() here as it's already called in the loop function
}

void setup(void) {
  ultrasonicSetup();
  IRSetup();

  Serial.begin(9600);
  Serial3.begin(9600);

  u8g.begin();
}

void loop(void) {
  while (Serial3.available() > 0) {
    char c = Serial3.read();
    if (gps.encode(c)) {
      displayInfo();
    }
  }

  unsigned long currentMillis = millis();

  // Update OLED display
  if (currentMillis - lastUltrasonicUpdate >= ultrasonicInterval) {
    lastUltrasonicUpdate = currentMillis;
    u8g.firstPage();
    do {
      draw();
    } while (u8g.nextPage());
    ultrasonicLoop();
  }

  // Update IR
  if (currentMillis - lastIRUpdate >= IRInterval) {
    lastIRUpdate = currentMillis;
    IRLoop();
  }
}
