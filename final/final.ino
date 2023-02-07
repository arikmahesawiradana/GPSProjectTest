#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

float latitude;     //Storing the Latitude
float longitude;    //Storing the Longitude
float velocity;     //Variable  to store the velocity
float sats;         //Variable to store no. of satellites response
String bearing;     //Variable to store orientation or direction of GPS
unsigned int move_index = 1;       // fixed location for now
static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

//WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget
WiFiUDP Udp;
TinyGPSPlus gps;      // The TinyGPS++ object
SoftwareSerial mygps(RXPin, TXPin);  // The serial connection to the GPS device

void checkGPS()
{
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
//    Blynk.virtualWrite(V3, "GPS ERROR");  // Value Display widget  on V3 if GPS not detected
  }
}

void displayInfo()
{
  if (gps.location.isValid() )
  {
    sats = gps.satellites.value();       //get number of satellites
    latitude = (gps.location.lat());     //Storing the Lat. and Lon.
    longitude = (gps.location.lng());
    velocity = gps.speed.kmph();         //get velocity
    bearing = TinyGPSPlus::cardinal(gps.course.value());     // get the direction
 
    Serial.print("SATS:  ");
    Serial.println(sats);  // float to x decimal places
    Serial.print("LATITUDE:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONGITUDE: ");
    Serial.println(longitude, 6);
    Serial.print("SPEED: ");
    Serial.print(velocity);
    Serial.println("kmph");
    Serial.print("DIRECTION: ");
    Serial.println(bearing);

    Udp.write(char(latitude));
    Udp.write(char(longitude));
    Udp.write(char(velocity));
//    Udp.write(bearing);
    Udp.write(char(sats));

 /*
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    Blynk.virtualWrite(V3, sats);
    Blynk.virtualWrite(V4, velocity);
    Blynk.virtualWrite(V5, bearing);
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    */
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mygps.begin(GPSBaud);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("AutoConnectAP","password");
  if(!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
    Udp.endPacket();
    } 
    else {
      Udp.beginPacket("255.255.255.255", 14550); //for udp using port 14550 and ip broadcast 255.255.255.255
      Udp.write("Connected");
      Serial.println("UDP is starting");
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (mygps.available() > 0)
  {
    // sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(mygps.read()))
      displayInfo();
  }
}
