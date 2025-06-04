# 🐾 HỆ THỐNG PHÁT HIỆN VÀ ĐẾM SỐ LƯỢNG ĐỘNG VẬT

Dự án này triển khai một hệ thống **tự động phát hiện và đếm vật nuôi (chó, mèo)** sử dụng **ESP32-CAM** để chụp ảnh, **YOLOv8** để nhận diện và đếm, và một **giao diện web** để hiển thị kết quả theo thời gian thực. Ảnh có thể được tải lên **Roboflow** để huấn luyện lại mô hình (tuỳ chọn). Hệ thống được thiết kế chi phí thấp, phù hợp cho việc giám sát vật nuôi theo thời gian thực tại nhà, trang trại, hoặc trạm cứu hộ.

---

## 📊 Tổng Quan Hệ Thống

Hệ thống bao gồm các thành phần chính:
- **ESP32-CAM:** Thiết bị chụp ảnh mỗi 5 giây, gửi ảnh đến server Python qua HTTP POST.
- **Server Python:** Xử lý ảnh bằng YOLOv8 để phát hiện và đếm chó/mèo, lưu ảnh cục bộ, cung cấp API cho giao diện web.
- **Giao diện Web:** Trang HTML hiển thị số lượng chó/mèo theo thời gian thực.
- **Roboflow (Tùy chọn):** Tải ảnh lên để tạo tập dữ liệu và huấn luyện lại mô hình.

> **Lưu ý:** ESP32-CAM sử dụng chân **GPIO 16** (không phải GPIO 22 mặc định) cho SIOC (SCL) của camera để tránh xung đột.

---

## 🛠️ Yêu Cầu Phần Cứng

- **ESP32-CAM (AI-Thinker):** Tích hợp camera OV2640.
- **Bộ chuyển USB-to-Serial (FTDI):** CP2102 hoặc CH340, hỗ trợ 5V, 500mA+.
- **Dây Jumper:** 4-5 dây đực-đực hoặc đực-cái.
- **Thẻ MicroSD (tuỳ chọn):** 4-32GB, FAT32 để lưu ảnh cục bộ.
- **Nguồn ngoài (tuỳ chọn):** Adapter 5V hoặc pin LiPo 3.7V + module tăng áp.
- **Máy tính:** Để lập trình và chạy server.
- **Mạng WiFi:** Kết nối ổn định để truyền ảnh.

---

## 💻 Yêu Cầu Phần Mềm

- **Arduino IDE:** Lập trình ESP32-CAM.
- **Python 3.8+:** Chạy server.
- **Trình duyệt web:** Xem giao diện web.

### Thư viện Arduino
- `WiFi` (tích hợp trong core ESP32)
- `HTTPClient` (tích hợp trong core ESP32)
- `esp_camera` (cài qua Library Manager)

### Thư viện Python
- `flask`
- `flask-cors`
- `ultralytics`
- `opencv-python`
- `requests`
- `pillow`
- `roboflow` (tuỳ chọn)

---

## 🚀 Hướng Dẫn Cài Đặt

### 1. Cài Đặt Arduino IDE và ESP32-CAM

1. **Cài Arduino IDE**: [Tải tại đây](https://www.arduino.cc/en/software)
2. **Thêm ESP32 vào Boards Manager:**
   - Vào **File > Preferences**.
   - Thêm vào Additional Boards Manager URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json
     ```
   - Vào **Tools > Board > Boards Manager**, tìm **ESP32** và cài đặt.
3. **Cài thư viện `esp_camera`:**
   - Vào **Sketch > Include Library > Manage Libraries**.
   - Tìm `esp_camera` và cài đặt.
4. **Chọn Board & Cổng COM:**
   - **Tools > Board:** Chọn **AI-Thinker ESP32-CAM**.
   - **Tools > Port:** Chọn đúng cổng COM.

### 2. Cấu Hình ESP32-CAM

1. Tải repository về máy.
2. Mở file **esp32_cam_pet_counter.ino** trong Arduino IDE.
3. **Cập nhật thông tin trong code:**
   - Thay `YOUR_WIFI_SSID` và `YOUR_WIFI_PASSWORD` bằng WiFi của bạn.
   - Thay `your-server-ip` trong `serverUrl` bằng địa chỉ IP của server (ví dụ: `http://192.168.1.100:5000/process_image`).
4. Kết nối ESP32-CAM với FTDI (GPIO 0 nối GND để nạp code).
5. Tải code lên ESP32-CAM.
6. Ngắt GPIO 0 khỏi GND sau khi nạp xong.

### 3. Cài Đặt Server Python

1. Đảm bảo **Python 3.8+** đã được cài đặt ([python.org](https://www.python.org/)).
2. Cài các thư viện cần thiết:
   ```bash
   pip install flask flask-cors ultralytics opencv-python requests pillow roboflow
   ```
3. Mở file **pet_counter_server.py** và cập nhật (nếu dùng Roboflow):
   - `ROBOFLOW_API_KEY`: API key từ Roboflow.
   - `ROBOFLOW_WORKSPACE`: Tên workspace.
   - `ROBOFLOW_PROJECT`: Tên project.
4. Chạy server:
   ```bash
   python pet_counter_server.py
   ```
   > Mặc định server chạy trên **http://0.0.0.0:5000**

### 4. Cài Đặt Giao Diện Web

1. Lưu file **web_client.html** vào một thư mục.
2. Cập nhật URL trong **web_client.html**:
   - Thay `your-server-ip` bằng IP server (ví dụ: `http://192.168.1.100:5000/get_pet_count`).
3. Chạy web server:
   ```bash
   python -m http.server 8000
   ```
4. Mở trình duyệt và truy cập: [http://localhost:8000/web_client.html](http://localhost:8000/web_client.html)

---

## 🐕‍🦺 Hình Ảnh Minh Họa

<p align="center">
  <img src="images/web_demo.png" width="600" alt="Giao diện web hiển thị số lượng chó/mèo">
</p>

---

## ⚡ (Tuỳ chọn) Kết nối Roboflow

1. Đăng ký tài khoản tại [app.roboflow.com](https://app.roboflow.com).
2. Tạo project mới để phát hiện vật nuôi (ví dụ: `Phat-Hien-Vat-Nuoi`).
3. Lấy API key, workspace, và project name từ dashboard Roboflow.
4. Cập nhật các thông tin này trong **pet_counter_server.py**.
5. Ảnh sẽ tự động tải lên Roboflow để huấn luyện.
6. Sau khi gắn nhãn và huấn luyện, tải mô hình (`best.pt`) và thay thế **yolov8n.pt** trong **pet_counter_server.py**.

---

## 🏃‍♂️ Chạy Hệ Thống

1. Cấp nguồn cho ESP32-CAM (qua FTDI hoặc nguồn 5V ngoài).
2. Chạy server Python:
   ```bash
   python pet_counter_server.py
   ```
3. Mở giao diện web tại: [http://localhost:8000/web_client.html](http://localhost:8000/web_client.html)
4. ESP32-CAM sẽ tự động chụp ảnh mỗi 5 giây, gửi lên server, và giao diện web hiển thị số lượng chó/mèo theo thời gian thực.

---

---

## ⭐ Góp Ý & Đóng Góp

Mọi ý kiến đóng góp, pull request đều được hoan nghênh! Hãy để lại star ⭐ nếu bạn thấy dự án hữu ích.

---

<p align="center">
  <b>🐾 Chúc bạn thành công với dự án giám sát vật nuôi thông minh! 🐾</b>
</p>
