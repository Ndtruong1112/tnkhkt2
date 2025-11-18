#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("DA TAO WIFI AP!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP ESP32 (AP): ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  int clients = WiFi.softAPgetStationNum();
  Serial.print("So client ket noi: ");
  Serial.println(clients);
  delay(2000);
}