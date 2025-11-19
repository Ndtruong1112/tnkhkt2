# Bài tập Thực hành Lập trình Nhúng & IoT với ESP32

Repository này chứa mã nguồn và tài liệu cho các bài tập thực hành môn Lập trình Nhúng, sử dụng module ESP32.

## Mục lục

- [Giới thiệu](#giới-thiệu)
- [Cấu trúc thư mục](#cấu-trúc-thư-mục)
- [Bài tập 1: ESP32 Wi-Fi](#bài-tập-1-esp32-wi-fi)
- [Bài tập 2: ESP32 Bluetooth Low Energy (BLE)](#bài-tập-2-esp32-bluetooth-low-energy-ble)

---

## Giới thiệu
Dự án tập trung vào việc khai thác các tính năng kết nối không dây cốt lõi của ESP32:
1.  **Wi-Fi:** Kết nối Internet, gửi/nhận dữ liệu qua giao thức TCP/UDP.
2.  **BLE (Bluetooth Low Energy):** Tạo thiết bị ngoại vi (Peripheral), thiết bị trung tâm (Central) và thiết lập mạng giao tiếp giữa các thiết bị ESP32.

**Phần cứng sử dụng:**
-   KIT ESP32 (Dev Module).
-   Cáp Micro USB.
-   Điện thoại thông minh (Android/iOS) cài đặt ứng dụng **nRF Connect** hoặc **LightBlue**.

---

## Cấu trúc thư mục

```text
├── Exercise1/               # Bài tập 1: Wi-Fi
│   └── part1/               # Station Mode (TCP/UDP Client)
│       └── part1.ino
│
├── Exercise2/               # Bài tập 2: BLE
│   ├── part1/               # BLE Peripheral (Server)
│   │   └── BLE_Peripheral.ino
│   ├── part2/               # BLE Central (Client - Auto Connect)
│   │   └── BLE_Central.ino
│   └── part3/               # Client - Server System (Giao tiếp 2 ESP32)
│       ├── Server.ino       # Code cho ESP32 A
│       └── Client.ino       # Code cho ESP32 B
│
└── README.md                # File tài liệu tổng hợp này
