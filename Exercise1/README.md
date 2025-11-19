# Bài tập 1: ESP32 Wi-Fi

## Mục lục
- [Giới thiệu](#giới-thiệu)
- [Yêu cầu phần cứng & phần mềm](#yêu-cầu-phần-cứng--phần-mềm)
- [Hướng dẫn thực hiện](#hướng-dẫn-thực-hiện)
- [Mã nguồn (Source Code)](#mã-nguồn-source-code)
- [Giải thích mã nguồn](#giải-thích-mã-nguồn)

---

## Giới thiệu
Bài tập này hướng dẫn cấu hình ESP32 hoạt động ở chế độ **Station (STA)**. Thiết bị sẽ kết nối vào mạng Wi-Fi có sẵn, sau đó thực hiện kiểm tra các giao thức truyền nhận dữ liệu cơ bản trên Internet:
1.  **TCP Client:** Kết nối đến server `httpbin.org` để gửi yêu cầu HTTP.
2.  **UDP Client:** Gửi gói tin đến Google DNS (`8.8.8.8`) để kiểm tra giao thức không kết nối.

---

## Yêu cầu phần cứng & phần mềm
-   **Phần cứng:** 1 x Kit ESP32 (Dev Module), cáp Micro USB.
-   **Phần mềm:** Arduino IDE (đã cài đặt ESP32 Board Manager).
-   **Mạng:** Một điểm truy cập Wi-Fi (Router) có kết nối Internet.

---

## Hướng dẫn thực hiện
1.  Mở Arduino IDE và tạo file mới có tên `part1.ino`.
2.  Copy đoạn mã nguồn ở mục dưới vào file.
3.  **Quan trọng:** Sửa dòng `ssid` và `password` trong code thành tên và mật khẩu Wi-Fi của bạn.
4.  Kết nối ESP32 với máy tính, chọn đúng cổng COM.
5.  Nhấn nút **Upload** (mũi tên sang phải).
6.  Mở **Serial Monitor** (Ctrl + Shift + M), chỉnh tốc độ baud thành **115200** để xem kết quả.

---
## Giải thích mã nguồn
Giải thích mã nguồn
1. Thư viện sử dụng
WiFi.h: Thư viện chính để quản lý kết nối Wi-Fi trên ESP32.

WiFiClient.h: Cung cấp lớp đối tượng để tạo kết nối TCP (tương tự như trình duyệt web).

WiFiUdp.h: Cung cấp lớp đối tượng để gửi/nhận gói tin UDP (nhanh nhưng không đảm bảo thứ tự).

2. Quá trình kết nối (trong setup)
WiFi.begin(ssid, password): Bắt đầu quá trình kết nối vào Access Point.

WiFi.status(): Kiểm tra trạng thái. Vòng lặp while sẽ giữ chương trình dừng lại ở đây cho đến khi trạng thái chuyển sang WL_CONNECTED.

WiFi.localIP(): Trả về địa chỉ IP mà Router cấp cho ESP32 qua DHCP.

3. Hàm testTCP()
tcpClient.connect(host, port): Mở một kết nối TCP đến server (Handshake 3 bước).

tcpClient.println(...): Gửi dữ liệu text (HTTP Header) đi.

tcpClient.read(): Đọc từng byte dữ liệu trả về từ Server.

4. Hàm testUDP()
udp.beginPacket(ip, port): Chuẩn bị một gói tin UDP.

udp.endPacket(): Thực sự gửi gói tin đi.

udp.parsePacket(): Kiểm tra xem có gói tin nào được gửi ngược lại không.
## Mã nguồn (Source Code)
*File: `part1.ino`*

```cpp
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// === CẤU HÌNH WI-FI (SỬA TẠI ĐÂY) ===
const char* ssid = "TEN_WIFI_CUA_BAN";
const char* password = "MAT_KHAU_WIFI";

// Cấu hình Server để test
const char* tcpServer = "httpbin.org";
const int   tcpPort   = 80;
const char* udpServer = "8.8.8.8"; // Google DNS
const int   udpPort   = 53;

WiFiClient tcpClient;
WiFiUDP    udp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. KẾT NỐI WI-FI
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

  // 2. THỰC HIỆN TEST
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
  // Không làm gì trong loop
  delay(10000);
}
