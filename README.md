# ESP32 Wi-Fi

## Mục lục

- [Bài tập 1: ESP32 Wi-Fi Client (Station Mode)](#bài-tập-1-esp32-wi-fi-client-station-mode)

---

## Phần cứng & phần mềm chung
- **Phần cứng:** ESP32 (bất kỳ board nào hỗ trợ Arduino IDE).
- **Phần mềm:**
  - Arduino IDE với ESP32 core (cài qua Board Manager).
  - Serial Monitor (trong Arduino IDE).

---

# Bài tập 1: ESP32 Wi-Fi Client (Station Mode)

Tài liệu này mô tả bài tập thực hành với ESP32: Kết nối vào mạng Wi-Fi hiện có (station mode) và nhận IP qua DHCP.

Sau khi kết nối thành công, ESP32 sẽ thực hiện hai tác vụ để kiểm tra kết nối Internet:
1.  Hoạt động như một **TCP client** để kết nối và gửi yêu cầu HTTP GET đến server `httpbin.org` (cổng 80).
2.  Hoạt động như một **UDP client** để gửi một gói tin đến server DNS của Google (`8.8.8.8`, cổng 53).

Qua bài tập:
- Hiểu cách ESP32 kết nối Wi-Fi (STA) và nhận IP.
- Quan sát kết nối TCP đến một server public và nhận phản hồi.
- Quan sát cách gửi và nhận (hoặc không nhận) dữ liệu UDP.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// THAY ĐỔI thông tin Wi-Fi của bạn tại đây
const char* ssid = "Tang 2";
const char* password = "123123123";

// Cấu hình server public để test
const char* tcpServer = "httpbin.org";
const int   tcpPort   = 80;

const char* udpServer = "8.8.8.8"; // Google DNS
const int   udpPort   = 53;

WiFiClient tcpClient;
WiFiUDP    udp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("Dang ket noi WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Chờ kết nối
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nDA KET NOI WIFI!");
  Serial.print("IP ESP32: ");
  Serial.println(WiFi.localIP());

  // Thực hiện test TCP và UDP
  testTCP();
  testUDP();
}

void testTCP() {
  Serial.println("\n--- GUI TCP DEN SERVER ---");
  if (tcpClient.connect(tcpServer, tcpPort)) {
    Serial.println("TCP ket noi thanh cong!");
    // Gửi yêu cầu HTTP GET
    tcpClient.println("GET /get HTTP/1.1");
    tcpClient.println("Host: httpbin.org");
    tcpClient.println("Connection: close");
    tcpClient.println();

    delay(2000); // Chờ phản hồi
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

  delay(3000); // Chờ phản hồi
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
  // Không làm gì trong loop, chỉ chạy 1 lần trong setup
  delay(10000);
}
