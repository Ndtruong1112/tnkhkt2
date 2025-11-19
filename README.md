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

Dưới đây là tổ chức các file mã nguồn trong repository:

```text
.
├── Exercise1/               # Bài tập 1: Lập trình Wi-Fi
│   ├── part1/               # Chế độ Station (STA)
│   │   └── part1.ino        # Code kết nối Wi-Fi, TCP & UDP Client
│   └── part2/               # [Các bài tập mở rộng Wi-Fi nếu có]
│       ├── part2.ino
│       └── README.md
│
├── Exercise2/               # Bài tập 2: Lập trình BLE (Bluetooth Low Energy)
│   ├── part1/               # BLE Peripheral (Server)
│   │   └── part1.ino        # Code phát sóng BLE & gửi Notify nhiệt độ
│   ├── part2/               # BLE Central (Client)
│   │   └── part2.ino        # Code quét & tự động kết nối Server
│   ├── part3/               # Hệ thống giao tiếp Client - Server
│   │   ├── A/               # Code cho Board A (Server)
│   │   │   └── A.ino
│   │   └── B/               # Code cho Board B (Client)
│   │       └── B.ino
│   └── README.md            # Tài liệu hướng dẫn riêng cho Bài tập 2
│
└── README.md                # Tài liệu tổng hợp toàn bộ dự án
