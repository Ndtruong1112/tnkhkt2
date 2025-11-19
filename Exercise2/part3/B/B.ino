// ========================================================
// PART 3 - BLE CENTRAL (Client) - FULL FILE HOÀN CHỈNH
// Nhiệm vụ: Quét → Kết nối → Nhận Notify nhiệt độ → Điều khiển LED từ xa
// ========================================================

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("180F");
static BLEUUID tempUUID("2A19");
static BLEUUID ledUUID("0000ff01-0000-1000-8000-00805f9b34fb");

static BLEAdvertisedDevice* target = nullptr;
static BLERemoteCharacteristic* pTemp = nullptr;
static BLERemoteCharacteristic* pLed = nullptr;
static bool connected = false;

class AdvCB : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice dev) {
      if (dev.haveName() && dev.getName() == "ESP32_SERVER") {
        Serial.println("");
        Serial.println("==========================================");
        Serial.println("     TÌM THẤY ESP32_SERVER !");
        Serial.println("==========================================");
        BLEDevice::getScan()->stop();
        target = new BLEAdvertisedDevice(dev);
      }
    }
};

void notifyCB(BLERemoteCharacteristic* pChar, uint8_t* data, size_t len, bool isNotify) {
  if (len > 0) {
    float nhietdo = data[0];
    Serial.println("");
    Serial.println("╔══════════════════════════════════════╗");
    Serial.printf(  "║     NHẬT ĐỘ TỪ SERVER:  %.1f °C      ║\n", nhietdo);
    Serial.println("╚══════════════════════════════════════╝");
    Serial.println("");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(2000);

  Serial.println("\n\n=== PART 3 - ESP32 BLE CENTRAL (Client) ===");
  Serial.println("Đang quét tìm ESP32_SERVER (Peripheral)...\n");

  BLEDevice::init("");

  BLEScan* pScan = BLEDevice::getScan();
  pScan->setAdvertisedDeviceCallbacks(new AdvCB());
  pScan->setActiveScan(true);
  pScan->setInterval(100);
  pScan->setWindow(99);
  pScan->start(0);               // quét liên tục cho đến khi tìm thấy
}

void loop() {
  // Khi đã tìm thấy thiết bị → thực hiện kết nối 1 lần duy nhất
  if (target && !connected) {
    Serial.println("Đang kết nối đến ESP32_SERVER...");
    
    BLEClient* pClient = BLEDevice::createClient();
    
    if (pClient->connect(target)) {
      BLERemoteService* pService = pClient->getService(serviceUUID);
      if (pService) {
        pTemp = pService->getCharacteristic(tempUUID);
        pLed  = pService->getCharacteristic(ledUUID);

        if (pTemp && pTemp->canNotify()) {
          pTemp->registerForNotify(notifyCB);
          Serial.println("┌──────────────────────────────────────────┐");
          Serial.println("│  ĐÃ KẾT NỐI THÀNH CÔNG + BẬT NOTIFY OK!  │");
          Serial.println("└──────────────────────────────────────────┘\n");
          connected = true;
        } else {
          Serial.println("Lỗi: Không tìm thấy characteristic nhiệt độ hoặc không hỗ trợ Notify");
        }
      } else {
        Serial.println("Lỗi: Không tìm thấy Service 180F");
      }
    } else {
      Serial.println("Kết nối thất bại!");
    }
    
    target = nullptr;   // chỉ kết nối 1 lần
  }

  // Bonus: Gõ phím 1 hoặc 0 trên Serial Monitor để bật/tắt LED trên Server
  if (connected && Serial.available()) {
    char c = Serial.read();
    if (c == '1' || c == '0') {
      if (pLed && pLed->canWrite()) {
        pLed->writeValue(&c, 1);
        Serial.printf("→ Đã gửi lệnh: LED %s\n\n", c=='1'?"BẬT":"TẮT");
      }
    }
  }

  delay(100);
}