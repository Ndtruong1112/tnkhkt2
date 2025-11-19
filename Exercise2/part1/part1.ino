// Exercise2/part1/BLE_Peripheral.ino
// ESP32 làm BLE Server (Peripheral)
// Dùng app BLE trên điện thoại (nRF Connect, BLE Scanner, LightBlue...) để test

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUID tự tạo (có thể dùng tool online generate)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_READ  "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // Đọc giá trị (nhiệt độ giả lập)
#define CHARACTERISTIC_UUID_WRITE "1d6e8f1a-5f0f-4c31-9f43-7b6189d8e1e2"  // Ghi để bật/tắt LED

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicRead = NULL;
BLECharacteristic* pCharacteristicWrite = NULL;

bool deviceConnected = false;
uint8_t ledState = 0;           // 0 = tắt, 1 = bật
float fakeTemperature = 25.0;   // Giá trị nhiệt độ giả lập

// LED pin (thường dùng GPIO2 trên hầu hết board ESP32)
#define LED_PIN 2

// Callback khi có thiết bị kết nối/ngắt kết nối
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Thiết bị đã kết nối!");
      BLEDevice::startAdvertising(); // Cho phép kết nối thêm thiết bị khác nếu muốn
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Thiết bị đã ngắt kết nối!");
      BLEDevice::startAdvertising(); // Bắt đầu quảng bá lại
    }
};

// Callback khi nhận dữ liệu ghi từ client (điện thoại/ESP32 khác)
class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.print("Nhận dữ liệu từ client: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
        Serial.println();

        if (value[0] == '1') {
          digitalWrite(LED_PIN, HIGH);
          ledState = 1;
          Serial.println("=> LED BẬT");
        } else if (value[0] == '0') {
          digitalWrite(LED_PIN, LOW);
          ledState = 0;
          Serial.println("=> LED TẮT");
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Khởi động BLE Peripheral...");

  // Tạo thiết bị BLE
  BLEDevice::init("ESP32_BLE_GroupX");  // Đặt tên hiển thị trên điện thoại
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Tạo Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Characteristic 1: Đọc nhiệt độ (có notify)
  pCharacteristicRead = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_READ,
                          BLECharacteristic::PROPERTY_READ |
                          BLECharacteristic::PROPERTY_NOTIFY
                        );
  pCharacteristicRead->addDescriptor(new BLE2902()); // Descriptor để bật notify
  pCharacteristicRead->setValue("25.0"); // Giá trị ban đầu

  // Characteristic 2: Ghi để điều khiển LED
  pCharacteristicWrite = pService->createCharacteristic(
                           CHARACTERISTIC_UUID_WRITE,
                           BLECharacteristic::PROPERTY_WRITE
                         );
  pCharacteristicWrite->setCallbacks(new MyCharacteristicCallbacks());

  // Bắt đầu service
  pService->start();

  // Bắt đầu quảng bá (advertising)
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Đang quảng bá BLE, tên: ESP32_BLE_GroupX");
  Serial.println("Dùng app BLE trên điện thoại để kết nối và thử nghiệm!");
}

void loop() {
  // Cập nhật giá trị nhiệt độ giả lập mỗi 3 giây
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 3000) {
    lastUpdate = millis();
    fakeTemperature += (random(-10, 11) / 10.0); // Thay đổi ngẫu nhiên ±1°C
    if (fakeTemperature < 15) fakeTemperature = 15;
    if (fakeTemperature > 40) fakeTemperature = 40;

    String tempStr = String(fakeTemperature, 1);
    pCharacteristicRead->setValue(tempStr.c_str());
    
    // Nếu có thiết bị kết nối và bật notify thì sẽ tự động gửi
    pCharacteristicRead->notify();
    
    Serial.print("Nhiệt độ hiện tại: ");
    Serial.println(tempStr);
  }

  delay(100);
}