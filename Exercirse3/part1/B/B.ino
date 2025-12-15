// ==================== CON A ====================
#include <esp_now.h>
#include <WiFi.h>

// MAC của con B
uint8_t peerMac[] = {0x84, 0x1F, 0xE8, 0x69, 0xBE, 0x18};

String msg = "";

void OnDataSent(const esp_now_send_info_t *info, esp_now_send_status_t status) {
  Serial.print("GỬI: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "THÀNH CÔNG ✓" : "THẤT BẠI ✗");
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  Serial.println("\n╔══════════════════════════════════╗");
  Serial.println  ("  NHẬN TỪ B");
  Serial.println  ("╚══════════════════════════════════╝");
  Serial.print  ("   → ");
  Serial.write(data, len);
  Serial.println("\n");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, peerMac, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);

  Serial.println("\n=== ESP32 CON A ĐÃ SẴN SÀNG CHAT ===");
  Serial.println("MAC tôi: A0:B7:65:23:26:38");
  Serial.println("Đang kết nối với B (84:1F:E8:69:BE:18)");
  Serial.println("Gõ tin nhắn + Enter để gửi\n");
}

void loop() {
  if (Serial.available()) {
    msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      esp_now_send(peerMac, (uint8_t*)msg.c_str(), msg.length() + 1);
      Serial.println("→ A gửi: " + msg);
    }
  }
  delay(10);
}