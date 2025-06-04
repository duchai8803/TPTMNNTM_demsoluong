```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";      // Thay bằng SSID WiFi
const char* password = "YOUR_WIFI_PASSWORD"; // Thay bằng mật khẩu WiFi
const char* serverUrl = "http://your-server-ip:5000/process_image"; // Thay bằng IP server

// Cấu hình camera (AI-Thinker ESP32-CAM)
#define CAMERA_MODEL_AI_THINKER

// Tùy chỉnh chân camera (thay SIOC từ GPIO 22 sang GPIO 16)
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    16  // Thay GPIO 22 bằng GPIO 16
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32-CAM...");

  // Kết nối WiFi với timeout
  WiFi.begin(ssid, password);
  int wifi_timeout = 10000; // 10 giây
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < wifi_timeout) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed!");
    return;
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Cấu hình camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA; // 320x240 để tránh lỗi bộ nhớ
  config.jpeg_quality = 15; // 10-63, cao hơn = chất lượng thấp hơn
  config.fb_count = 1;

  // Khởi tạo camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    delay(1000);
    ESP.restart();
    return;
  }
  Serial.println("Camera initialized");
}

void loop() {
  // Chụp ảnh
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    return;
  }

  // Gửi ảnh đến server
  sendImageToServer(fb->buf, fb->len);

  // Giải phóng bộ nhớ
  esp_camera_fb_return(fb);

  delay(5000); // Chụp mỗi 5 giây
}

void sendImageToServer(uint8_t* image, size_t size) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(10000); // Timeout 10 giây
    http.begin(serverUrl);
    http.addHeader("Content-Type", "image/jpeg");

    Serial.println("Sending image to server...");
    int httpCode = http.POST(image, size);
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.printf("HTTP request failed: %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected");
  }
}
```