// ========================================================
// PART 3 - PERIPHERAL (Server) - FULL FILE ĐÃ SỬA HOÀN HẢO
// Chắc chắn Central sẽ thấy Service + kết nối ngay lập tức
// ========================================================

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define LED_PIN 2
#define SERVICE_UUID        "180F"
#define CHAR_TEMP_UUID      "2A19"
#define CHAR_LED_UUID       "0000ff01-0000-1000-8000-00805f9b34fb"

BLECharacteristic* pTempChar;
bool deviceConnected = false;
float temp = 25.0;

class ServerCB : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("\n[PART 3] CENTRAL ĐÃ KẾT NỐI THÀNH CÔNG!");
    }
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("[PART 3] Central ngắt → quảng bá lại...");
      delay(500);
      pServer->startAdvertising();   // QUAN TRỌNG!
    }
};

class LedCB : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override {
      uint8_t* d = pChar->getData();
      if (d) {
        digitalWrite(LED_PIN, (d[0]=='1') ? HIGH : LOW);
        Serial.printf("[PART 3] LED nhận lệnh: %c → %s\n", d[0], d[0]=='1'?"BẬT":"TẮT");
      }
    }
};

void setup() {
  Serial.begin(115200);
  while(!Serial); delay(1000);
  Serial.println("\n=== PART 3 - PERIPHERAL (Server) ===");
  pinMode(LED_PIN, OUTPUT);

  BLEDevice::init("ESP32_SERVER");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCB());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  pTempChar = pService->createCharacteristic(
                CHAR_TEMP_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pTempChar->addDescriptor(new BLE2902());
  pTempChar->setValue("25");

  pService->createCharacteristic(CHAR_LED_UUID,
                BLECharacteristic::PROPERTY_WRITE)->setCallbacks(new LedCB());

  pService->start();

  // ĐOẠN NÀY PHẢI CÓ ĐẦY ĐỦ → CENTRAL MỚI THẤY SERVICE!
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);   // QUAN TRỌNG NHẤT!
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->start();                         // bắt đầu quảng bá

  Serial.println("Đang phát với tên: ESP32_SERVER");
  Serial.println("Service UUID 180F đã được đưa vào advertising packet!");
}

void loop() {
  if (deviceConnected && millis() % 2000 < 100) {
    temp += random(-20, 21) / 10.0;
    temp = constrain(temp, 15, 45);
    uint8_t v = (uint8_t)temp;
    pTempChar->setValue(&v, 1);
    pTempChar->notify();
    Serial.printf("[PART 3] Notify nhiệt độ: %.1f °C\n", temp);
  }
  delay(100);
}