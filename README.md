# TPTMNNTM_demsoluong

Hệ Thống Phát Hiện và Đếm Vật Nuôi Tự Động với ESP32-CAM và YOLOv8

Dự án này triển khai một hệ thống tự động phát hiện và đếm vật nuôi (chó, mèo) sử dụng ESP32-CAM để chụp ảnh, YOLOv8 để nhận diện và đếm, và một giao diện web để hiển thị kết quả theo thời gian thực. Ảnh có thể được tải lên Roboflow để huấn luyện lại mô hình (tùy chọn). Hệ thống được thiết kế chi phí thấp, phù hợp cho việc giám sát vật nuôi theo thời gian thực.
Mục Lục

Tổng Quan
Yêu Cầu Phần Cứng
Yêu Cầu Phần Mềm
Hướng Dẫn Nối Dây
Hướng Dẫn Cài Đặt
Bước 1: Cài Đặt Arduino IDE
Bước 2: Cấu Hình ESP32-CAM
Bước 3: Cài Đặt Server Python
Bước 4: Cài Đặt Giao Diện Web
Bước 5: (Tùy Chọn) Cấu Hình Roboflow


Chạy Hệ Thống
Gỡ Lỗi
Đóng Góp


Tổng Quan
Hệ thống bao gồm:

ESP32-CAM: Chụp ảnh mỗi 5 giây và gửi đến server Python qua HTTP POST.
Server Python: Xử lý ảnh bằng YOLOv8 để phát hiện và đếm chó/mèo, lưu ảnh cục bộ, và cung cấp API cho giao diện web.
Giao Diện Web: Trang HTML hiển thị số lượng chó/mèo theo thời gian thực.
Roboflow (Tùy Chọn): Tải ảnh lên để tạo tập dữ liệu và huấn luyện lại mô hình.

ESP32-CAM sử dụng GPIO 16 (thay vì GPIO 22 mặc định) cho chân SIOC (SCL) của camera để tránh xung đột.
Yêu Cầu Phần Cứng

ESP32-CAM (AI-Thinker): Tích hợp camera OV2640.
Bộ Chuyển USB-to-Serial (FTDI): CP2102 hoặc CH340, hỗ trợ 5V, 500mA+.
Dây Jumper: 4-5 dây đực-đực hoặc đực-cái.
Thẻ MicroSD (Tùy Chọn): 4-32GB, định dạng FAT32, để lưu ảnh cục bộ.
Nguồn Ngoài (Tùy Chọn): Adapter 5V hoặc pin LiPo 3.7V với module tăng áp.
Máy Tính: Để lập trình và chạy server.
Mạng WiFi: Kết nối ổn định để truyền ảnh.

Yêu Cầu Phần Mềm

Arduino IDE: Lập trình ESP32-CAM.
Python 3.8+: Chạy server.
Trình Duyệt Web: Xem giao diện web.
Thư viện Arduino:
WiFi (tích hợp trong core ESP32)
HTTPClient (tích hợp trong core ESP32)
esp_camera (cài qua Library Manager)


Thư viện Python:
flask
flask-cors
ultralytics
opencv-python
requests
pillow
roboflow (tùy chọn)



Hướng Dẫn Nối Dây
Kết nối ESP32-CAM với bộ FTDI để lập trình và cấp nguồn:



Chân ESP32-CAM
Chân FTDI
Ghi Chú



5V
5V
Cấp nguồn 5V


GND
GND
Nối đất


U0R (RX)
TX
Serial nhận


U0T (TX)
RX
Serial gửi


GPIO 0
GND
Chế độ flash (chỉ nối khi lập trình)


Sơ Đồ ASCII:
ESP32-CAM                FTDI
[5V] ------------------- [5V]
[GND] ------------------ [GND]
[U0R/RX] --------------- [TX]
[U0T/TX] --------------- [RX]
[GPIO 0] --[Công tắc]-- [GND] (Chỉ nối khi lập trình)

Ghi Chú:

Đảm bảo FTDI đặt ở chế độ 5V (kiểm tra jumper).
Camera OV2640 đã tích hợp, không cần nối dây thêm.
Thẻ microSD (nếu dùng) gắn trực tiếp vào khe trên ESP32-CAM (định dạng FAT32).

Hướng Dẫn Cài Đặt
Bước 1: Cài Đặt Arduino IDE

Tải và cài đặt Arduino IDE.
Thêm hỗ trợ board ESP32:
Vào File > Preferences.
Thêm vào Additional Boards Manager URLs:https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json


Vào Tools > Board > Boards Manager, tìm "ESP32" và cài đặt.


Cài thư viện esp_camera:
Vào Sketch > Include Library > Manage Libraries.
Tìm "esp_camera" và cài đặt.


Chọn board:
Vào Tools > Board, chọn AI-Thinker ESP32-CAM.
Chọn cổng COM đúng trong Tools > Port.



Bước 2: Cấu Hình ESP32-CAM

Tải repository này về hoặc sao chép.
Mở file esp32_cam_pet_counter.ino trong Arduino IDE.
Cập nhật các thông tin trong code:
Thay YOUR_WIFI_SSID và YOUR_WIFI_PASSWORD bằng thông tin WiFi của bạn.
Thay your-server-ip trong serverUrl bằng địa chỉ IP của server (ví dụ: http://192.168.1.100:5000/process_image).


Kết nối ESP32-CAM với FTDI (đảm bảo GPIO 0 nối với GND).
Tải code lên ESP32-CAM.
Ngắt kết nối GPIO 0 khỏi GND sau khi tải xong.

Bước 3: Cài Đặt Server Python

Đảm bảo Python 3.8+ đã được cài đặt (python.org).
Cài các thư viện cần thiết:pip install flask flask-cors ultralytics opencv-python requests pillow roboflow


Mở file pet_counter_server.py và cập nhật (nếu dùng Roboflow):
ROBOFLOW_API_KEY: Khóa API Roboflow của bạn.
ROBOFLOW_WORKSPACE: Tên workspace Roboflow.
ROBOFLOW_PROJECT: Tên dự án Roboflow.


Chạy server:python pet_counter_server.py

Server sẽ chạy trên http://0.0.0.0:5000.

Bước 4: Cài Đặt Giao Diện Web

Lưu file web_client.html vào một thư mục.
Cập nhật URL lấy dữ liệu trong web_client.html:
Thay your-server-ip bằng IP server (ví dụ: http://192.168.1.100:5000/get_pet_count).


Chạy web server:python -m http.server 8000


Mở trình duyệt và truy cập http://localhost:8000/web_client.html.

Bước 5: (Tùy Chọn) Cấu Hình Roboflow

Đăng ký tài khoản tại app.roboflow.com.
Tạo dự án mới cho phát hiện vật nuôi (ví dụ: "Phat-Hien-Vat-Nuoi").
Lấy API key, workspace, và project name từ dashboard Roboflow.
Cập nhật các thông tin này trong pet_counter_server.py.
Ảnh sẽ tự động tải lên Roboflow để huấn luyện.
Sau khi gắn nhãn và huấn luyện, tải mô hình (best.pt) và thay thế yolov8n.pt trong pet_counter_server.py.

Chạy Hệ Thống

Cấp nguồn cho ESP32-CAM (qua FTDI hoặc nguồn 5V).
Chạy server Python:python pet_counter_server.py


Mở giao diện web tại http://localhost:8000/web_client.html.
ESP32-CAM chụp ảnh mỗi 5 giây, gửi đến server, và giao diện web cập nhật số lượng chó/mèo theo thời gian thực.

Gỡ Lỗi

Arduino (Serial Monitor, 115200 baud):
WiFi connection failed!: Kiểm tra ssid, password hoặc tín hiệu WiFi.
Camera init failed with error 0x...: Đảm bảo camera OV2640 gắn chắc hoặc thử GPIO khác cho SIOC (ví dụ: GPIO 17).
HTTP request failed: Xác minh server đang chạy tại serverUrl.


Server Python:
Kiểm tra logs để xem ảnh có được xử lý hoặc tải lên Roboflow không.
Đảm bảo cổng 5000 mở (tắt firewall hoặc dùng ufw allow 5000 trên Linux).


Giao Diện Web:
Mở console trình duyệt (F12) để kiểm tra lỗi fetch.
Đảm bảo IP server trong web_client.html đúng.


Phần Cứng:
Kiểm tra FTDI cung cấp 5V, 500mA+.
Xác minh kết nối camera OV2640 trên ESP32-CAM.



Đóng Góp
Chúng tôi hoan nghênh mọi đóng góp! Vui lòng:

Fork repository này.
Tạo nhánh mới (git checkout -b tinh-nang/ten-tinh-nang).
Commit thay đổi (git commit -m "Thêm tính năng XYZ").
Push lên nhánh (git push origin tinh-nang/ten-tinh-nang).
Mở Pull Request.

