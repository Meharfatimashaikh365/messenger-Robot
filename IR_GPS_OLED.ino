#include "U8glib.h"
#include <TinyGPS++.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
TinyGPSPlus gps;

const int IRSensor = 2; // Connect the digital output pin to Arduino digital pin 2

void u8g_prepare(void) 
{
  u8g.setFont(u8g_font_4x6); // Change font size to 4x6
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void displayLocationOnOLED()
{
  if (gps.location.isValid())
  {
    u8g.setFont(u8g_font_unifont);

    // Display Latitude on OLED
    u8g.drawStr(2, 10, "Lat: ");
    char latitude[15];
    dtostrf(gps.location.lat(), 11, 7, latitude);
    u8g.drawStr(40, 10, latitude); // Adjusted X-coordinate for left-align

    // Display Longitude on OLED
    u8g.drawStr(2, 25, "Long: ");
    char longitude[15];
    dtostrf(gps.location.lng(), 11, 7, longitude);
    u8g.drawStr(40, 25, longitude); // Adjusted X-coordinate for left-align
  }
  else
  {
    u8g.setFont(u8g_font_4x6); // Change font size to 4x6 for the "Location: Not Available" message
    u8g.drawStr(2, 10, "Location: N/A");
  }
}

void draw(void) 
{
  u8g_prepare();
  displayLocationOnOLED();
}

void displayInfo()
{
  // ... (unchanged code for other information)

  // Display latitude and longitude on the serial monitor
  Serial.print("Lat: ");
  Serial.println(gps.location.lat(), 7);
  Serial.print("Long: ");
  Serial.println(gps.location.lng(), 7);

  // IR Sensor
  int sensorValue = digitalRead(IRSensor); // Read digital value from IR sensor
  Serial.print("IR Sensor: ");
  Serial.println(sensorValue == LOW ? "Object Detected!" : "No Object Detected.");

  Serial.println();
  Serial.println();
  
}

void setup(void) 
{
  // Begin serial communication
  Serial.begin(9600);
  Serial3.begin(9600);

  // IR Sensor
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
}

void loop(void) 
{
  // GPS
  while (Serial3.available() > 0)
    if (gps.encode(Serial3.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }

  // OLED display
  u8g.firstPage();  
  do 
  {
    draw();
  } while( u8g.nextPage() );
}
