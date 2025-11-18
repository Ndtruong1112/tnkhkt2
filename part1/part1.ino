#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

const char* ssid = "Tang 2";
const char* password = "123123123";

const char* tcpServer = "httpbin.org";
const int   tcpPort   = 80;

const char* udpServer = "8.8.8.8";
const int   udpPort   = 53;

WiFiClient tcpClient;
WiFiUDP    udp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Dang ket noi WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nDA KET NOI WIFI!");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());

  testTCP();
  testUDP();
}

void testTCP() {
  Serial.println("\n--- GUI TCP DEN SERVER ---");
  if (tcpClient.connect(tcpServer, tcpPort)) {
    Serial.println("TCP ket noi thanh cong!");
    tcpClient.println("GET /get HTTP/1.1");
    tcpClient.println("Host: httpbin.org");
    tcpClient.println("Connection: close");
    tcpClient.println();

    delay(2000);
    Serial.println("Phan hoi TCP:");
    while (tcpClient.available()) {
      Serial.write(tcpClient.read());
    }
    tcpClient.stop();
  } else {
    Serial.println("TCP ket noi that bai!");
  }
}

void testUDP() {
  Serial.println("\n--- GUI UDP DEN SERVER ---");
  udp.begin(udpPort);

  IPAddress serverIP(8,8,8,8);
  String message = "Hello UDP from ESP32!";
  udp.beginPacket(serverIP, udpPort);
  udp.print(message);
  udp.endPacket();
  Serial.println("Da gui UDP: " + message);

  delay(3000);
  if (udp.parsePacket()) {
    char buffer[255];
    int len = udp.read(buffer, 255);
    buffer[len] = '\0';
    Serial.print("Phan hoi UDP: ");
    Serial.println(buffer);
  } else {
    Serial.println("Khong nhan duoc phan hoi UDP");
  }
  udp.stop();
}

void loop() {
  delay(10000);
}