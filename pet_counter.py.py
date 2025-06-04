import cv2
import numpy as np
from ultralytics import YOLO
import requests
from flask import Flask, request, jsonify
import threading
import time

# Khởi tạo Flask app
app = Flask(__name__)

# Khởi tạo model YOLO
model = YOLO('yolov8n.pt')  # Sử dụng YOLOv8 nano (có thể thay bằng yolov8s.pt, yolov8m.pt...)

# URL của web server để gửi kết quả
WEB_SERVER_URL = "http://your-web-server/api/pet_count"

# Danh sách các class liên quan đến vật nuôi
PET_CLASSES = ['cat', 'dog']  # Có thể mở rộng thêm: 'bird', 'fish', etc.

# Biến toàn cục để lưu kết quả đếm
pet_count = {'cats': 0, 'dogs': 0}

def process_frame(frame):
    """Xử lý frame từ camera và đếm vật nuôi"""
    global pet_count
    
    # Dự đoán với YOLO
    results = model(frame)
    
    # Reset bộ đếm
    pet_count = {'cats': 0, 'dogs': 0}
    
    # Đếm số lượng vật nuôi
    for result in results:
        for box in result.boxes:
            class_name = model.names[int(box.cls)]
            if class_name in PET_CLASSES:
                pet_count[class_name + 's'] += 1
    
    # Gửi kết quả lên web server
    send_to_server()

def send_to_server():
    """Gửi kết quả đếm lên web server"""
    try:
        response = requests.post(WEB_SERVER_URL, json=pet_count)
        if response.status_code == 200:
            print(f"Đã gửi: {pet_count}")
        else:
            print(f"Lỗi khi gửi: {response.status_code}")
    except Exception as e:
        print(f"Lỗi kết nối server: {e}")

@app.route('/get_pet_count', methods=['GET'])
def get_pet_count():
    """API để lấy số lượng vật nuôi hiện tại"""
    return jsonify(pet_count)

def camera_thread():
    """Thread xử lý camera"""
    cap = cv2.VideoCapture(0)  # Mở webcam (0 là camera mặc định)
    
    if not cap.isOpened():
        print("Không thể mở camera")
        return
    
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Không thể đọc frame")
            break
            
        # Xử lý frame
        process_frame(frame)
        
        # Hiển thị frame với kết quả (tùy chọn)
        cv2.imshow('Pet Detection', frame)
        
        # Thoát nếu nhấn 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
        # Giãn cách xử lý để không quá tải
        time.sleep(1)
    
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    # Khởi động thread camera
    camera = threading.Thread(target=camera_thread)
    camera.daemon = True
    camera.start()
    
    # Khởi động Flask server
    app.run(host='0.0.0.0', port=5000)