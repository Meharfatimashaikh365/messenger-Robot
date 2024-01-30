#include <TinyGPS++.h>

TinyGPSPlus gps;

int serialNumber = 0; // Serial number counter

// Room coordinates and deviation
double RC1_lat = 24.934001847023765;
double RC1_long = 67.11231171254454;
double RC1_deviation = 0.0002988221736035993;

void displayLocationOnSerial() {
  if (gps.location.isValid()) {
    // Check if location is within range
    if ((gps.location.lat() >= RC1_lat - RC1_deviation) && (gps.location.lat() <= RC1_lat + RC1_deviation) &&
        (gps.location.lng() >= RC1_long - RC1_deviation) && (gps.location.lng() <= RC1_long + RC1_deviation)) {
      Serial.print("GPS Location - Lat: ");
      Serial.print(gps.location.lat(), 14); // Increase precision
      Serial.print(" - Long: ");
      Serial.println(gps.location.lng(), 14); // Increase precision
          } else {
      Serial.print("GPS Location - Lat: ");
      Serial.print(gps.location.lat(), 14); // Increase precision
      Serial.print(" - Long: ");
      Serial.println(gps.location.lng(), 14); // Increase precision
    
    }
  } else {
    Serial.println("Location: N/A");
  }
}

void displayInfo() {
  // Display serial number
  Serial.print("Serial: ");
  Serial.print(serialNumber);

  // Display latitude and longitude on the serial monitor
  Serial.print(" - Lat: ");
  Serial.print(gps.location.lat(), 14); // Increase precision
  Serial.print(" - Long: ");
  Serial.println(gps.location.lng(), 14); // Increase precision

  delay(500); // Wait for 0.5 seconds

  // Increment serial number
  serialNumber++;
}

void setup(void) {
  // Begin serial communication
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop(void) {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial3.available() > 0)
    if (gps.encode(Serial3.read()))
      //displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true);
  }

  // Print GPS location on Serial monitor
  displayLocationOnSerial();
}
