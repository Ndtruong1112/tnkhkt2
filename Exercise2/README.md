# Bài tập 2: ESP32 Bluetooth Low Energy (BLE)

## Mục lục
- [Giới thiệu](#giới-thiệu)
- [Phần 1: BLE Peripheral (Server)](#phần-1-ble-peripheral-server)
- [Phần 2: BLE Central (Client)](#phần-2-ble-central-client)
- [Phần 3: Hệ thống giao tiếp Client - Server](#phần-3-hệ-thống-giao-tiếp-client---server)

---

## Giới thiệu
Bài tập này tập trung vào kỹ thuật giao tiếp không dây tiết kiệm năng lượng (BLE) trên ESP32. Sinh viên sẽ thực hành cấu hình ESP32 thành các vai trò khác nhau:
1.  **Peripheral (Server):** Phát sóng để thiết bị khác kết nối.
2.  **Central (Client):** Chủ động quét và kết nối tới thiết bị khác.
3.  **Giao tiếp M2M (Machine to Machine):** Hai board ESP32 giao tiếp trực tiếp với nhau.

---

## Phần 1: BLE Peripheral (Server)
*Thư mục: `Exercise2/part1/`*

Trong phần này, ESP32 hoạt động như một thiết bị ngoại vi (GATT Server). Nó phát tín hiệu quảng bá để điện thoại thông minh có thể tìm thấy và kết nối.

### Chức năng
* [cite_start]**Tên thiết bị:** `ESP32_BLE_GroupX` [cite: 50]
* [cite_start]**Service UUID:** `4fafc201-1fb5-459e-8fcc-c5c9c331914b` [cite: 37]
* **Đặc tính (Characteristics):**
    1.  [cite_start]**Read (Notify):** Gửi dữ liệu nhiệt độ giả lập lên điện thoại mỗi 3 giây. [cite: 52, 53, 59-62]
    2.  [cite_start]**Write:** Nhận lệnh từ điện thoại (`1` hoặc `0`) để Bật/Tắt đèn LED (GPIO 2). [cite: 54, 47-49]

### Hướng dẫn kiểm tra
1.  Nạp code `BLE_Peripheral.ino` vào ESP32.
2.  Mở app **nRF Connect** hoặc **LightBlue** trên điện thoại.
3.  Quét và kết nối với thiết bị tên `ESP32_BLE_GroupX`.
4.  Đăng ký nhận Notify để thấy giá trị nhiệt độ thay đổi.
5.  Ghi giá trị `1` (dạng text/utf8) vào đặc tính Write để bật LED.

---

## Phần 2: BLE Central (Client)
*Thư mục: `Exercise2/part2/`*

Trong phần này, ESP32 đóng vai trò là thiết bị trung tâm (GATT Client). Nó tự động quét môi trường để tìm kiếm một Server cụ thể.

### Chức năng
* [cite_start]**Tự động quét:** Tìm kiếm thiết bị có tên là `ESP32_BLE`. [cite: 66]
* [cite_start]**Tự động kết nối:** Khi tìm thấy đúng tên, nó sẽ dừng quét và kết nối ngay lập tức. [cite: 67, 69]
* [cite_start]**Gửi lệnh định kỳ:** Sau khi kết nối, Client tự động gửi lệnh bật/tắt LED sang Server mỗi 10 giây. [cite: 74-75]

### Lưu ý
Code này được lập trình để tìm thiết bị tên `ESP32_BLE`. Nếu bạn muốn nó kết nối với thiết bị ở Phần 1, hãy sửa tên trong code thành `ESP32_BLE_GroupX`.

---

## Phần 3: Hệ thống giao tiếp Client - Server
*Thư mục: `Exercise2/part3/`*

Đây là bài tập tổng hợp, sử dụng 2 board ESP32 để tạo thành một hệ thống IoT thu nhỏ hoàn chỉnh.

### Cấu hình
* **Board A (Server):** Nạp file `Server.ino`.
    * [cite_start]Tên thiết bị: `ESP32_SERVER`. [cite: 8]
    * [cite_start]Nhiệm vụ: Đo nhiệt độ (giả lập) gửi đi và nhận lệnh điều khiển LED. [cite: 13-14, 6]
* **Board B (Client):** Nạp file `Client.ino`.
    * [cite_start]Nhiệm vụ: Tìm `ESP32_SERVER` để kết nối. [cite: 19]
    * [cite_start]Hiển thị: Nhận nhiệt độ từ Server và in ra Serial Monitor. [cite: 21-23]
    * [cite_start]Điều khiển: Người dùng nhập phím `1` hoặc `0` vào Serial Monitor của Client để bật/tắt LED trên Server từ xa. [cite: 34-36]

### Kết quả thực hiện
1.  [cite_start]Client báo `TÌM THẤY ESP32_SERVER` và kết nối thành công. [cite: 20, 29]
2.  [cite_start]Trên Serial Monitor của Client liên tục hiện cập nhật nhiệt độ: `NHIỆT ĐỘ TỪ SERVER: 25.5 °C`. [cite: 22]
3.  Gõ `1` vào Client -> LED trên Server sáng ngay lập tức.

---

## Yêu cầu phần cứng
* **Phần 1 & 2:** 01 ESP32 + Điện thoại thông minh.
* **Phần 3:** 02 ESP32 (Một làm Server, một làm Client).

