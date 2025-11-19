
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("180F");
static BLEUUID charTempUUID("2A19");
static BLEUUID charLedUUID("0000ff01-0000-1000-8000-00805f9b34fb");

static BLEAdvertisedDevice* myDevice = nullptr;
static BLERemoteCharacteristic* pTempChar = nullptr;
static BLERemoteCharacteristic* pLedChar = nullptr;
static bool doConnect = false;
static bool connected = false;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveName() && advertisedDevice.getName() == "ESP32_BLE") {
        Serial.println("[SCAN] TÌM THẤY ESP32_BLE!");
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
      }
    }
};

void notifyCallback(BLERemoteCharacteristic* pChar, uint8_t* data, size_t len, bool isNotify) {
  if (len > 0) Serial.printf("[NOTIFY] Nhiệt độ: %d °C\n", data[0]);
}

bool connectToServer() {
  BLEClient* pClient = BLEDevice::createClient();
  if (!pClient->connect(myDevice)) return false;

  BLERemoteService* pService = pClient->getService(serviceUUID);
  if (!pService) { pClient->disconnect(); return false; }

  pTempChar = pService->getCharacteristic(charTempUUID);
  if (pTempChar && pTempChar->canNotify()) pTempChar->registerForNotify(notifyCallback);

  pLedChar = pService->getCharacteristic(charLedUUID);

  Serial.println("[BLE] KẾT NỐI THÀNH CÔNG!");
  return true;
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(2000);  // ← quan trọng: chờ Part 1 khởi động trước
  Serial.println("\n=== PART 2 - BLE CENTRAL  ===");
  Serial.println("Đang quét ESP32_BLE...\n");

  BLEDevice::init("");
}

void loop() {
  if (!connected && !doConnect) {
    BLEScan* pScan = BLEDevice::getScan();
    pScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pScan->setActiveScan(true);
    pScan->start(0);  // quét vô hạn, không bị timeout 
  }

  if (doConnect) {
    connectToServer();
    doConnect = false;
    connected = true;
  }

  if (connected && pLedChar && millis() % 10000 < 50) {  // mỗi 10s đổi trạng thái 1 lần
    static bool state = false;
    state = !state;
    pLedChar->writeValue(state ? "1" : "0");
    Serial.printf("→ Gửi lệnh: LED %s\n", state ? "BẬT" : "TẮT");
  }

  delay(100);
}