#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
void setup() {
  Serial.begin(115200);
  WiFi.begin("NAD NAD", "kupukupumalam");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(",");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  Udp.beginPacket("255.255.255.255", 8888);
  Udp.write("test1");
  Udp.endPacket();
  delay(10);
  Udp.beginPacket("255.255.255.255", 8888);
  Udp.write("test2");
  Udp.endPacket();
  delay(5000);
}
