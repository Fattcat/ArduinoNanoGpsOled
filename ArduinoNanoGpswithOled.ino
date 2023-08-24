#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET    -1
Adafruit_SSD1306 display(OLED_RESET);

SoftwareSerial serial_connection(5,6); //tx,rx 
TinyGPSPlus gps; // GPS object to process the NMEA data
unsigned long lastDisplayTime = 0;
const unsigned long displayInterval = 2000; // Zobraziť údaje každých 10 sekúnd

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicializácia OLED displeja
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.begin(9600);
  serial_connection.begin(9600);
  Serial.println("GPS Start");
}

void loop()
{
  while(serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }

  unsigned long currentMillis = millis();

  if(gps.location.isUpdated() && (currentMillis - lastDisplayTime >= displayInterval))
  {
    lastDisplayTime = currentMillis;
    
    display.clearDisplay();
    display.setTextSize(1);      // Veľkosť písma
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    display.print("Sat: ");
    display.println(gps.satellites.value());
    display.print("Lat: ");
    display.println(gps.location.lat(), 6);
    display.print("Lng: ");
    display.println(gps.location.lng(), 6);
    display.print("Alt(m): ");
    display.println(gps.altitude.meters(), 1);

    if (gps.date.isValid() && gps.time.isValid())
    {
      display.print("Date: ");
      display.print(gps.date.day());
      display.print(".");
      display.print(gps.date.month());
      display.print(".");
      display.println(gps.date.year());
      
      display.print("Time: ");
      int8_t adjustedHour = gps.time.hour() + 2; // časové pásmo (+2 hodiny)
      if (adjustedHour < 10) display.print("0");
      display.print(adjustedHour);
      display.print(":");
      if (gps.time.minute() < 10) display.print("0");
      display.print(gps.time.minute());
      display.print(":");
      if (gps.time.second() < 10) display.print("0");
      display.print(gps.time.second());
      display.display();
    }

    display.display();
  }
}
