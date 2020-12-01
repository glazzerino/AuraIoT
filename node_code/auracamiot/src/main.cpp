#include "FirebaseESP32.h"
FirebaseData firebaseData;
#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "Base64.h"

#include "esp_camera.h"

const char* ssid = "IZZI-9135";
const char* password = "chapie123";

String FIREBASE_HOST = "aura-iot-c6bc8.firebaseio.com";
String FIREBASE_AUTH = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";


String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++) {
        c=str.charAt(i);
        if (c == ' ') {
            encodedString+= '+';
        } else if (isalnum(c)) {
            encodedString+=c;
        } else {
            code1=(c & 0xf)+'0';
            if ((c & 0xf) >9) {
                code1=(c & 0xf) - 10 + 'A';
            }
            c=(c>>4)&0xf;
            code0=c+'0';
            if (c > 9) {
                code0=c - 10 + 'A';
            }
            code2='\0';
            encodedString+='%';
            encodedString+=code0;
            encodedString+=code1;
            //encodedString+=code2;
        }
        yield();
    }
    return encodedString;
}


String Photo2Base64() {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if(!fb) {
        Serial.println("Camera capture failed");
        return "";
    }

    String imageFile = "data:image/jpeg;base64,";
    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    for (int i=0; i<fb->len; i++) {
        base64_encode(output, (input++), 3);
        if (i%3==0) imageFile += urlencode(String(output));
    }
    // uint8_t* jpg = nullptr;
    // frame2jpg(fb, 50, &jpg, &fb->len);
    // Firebase.pushBlob(firebaseData, "/CAMERA/IMAGE", jpg, fb->len);
    esp_camera_fb_return(fb);

    return imageFile;
}

// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled

//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH 4 // FLASHLIGHT
void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    pinMode(FLASH, OUTPUT);
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();
    Serial.println("ssid: " + (String)ssid);
    Serial.println("password: " + (String)password);

    WiFi.begin(ssid, password);

    long int StartTime=millis();
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     if ((StartTime+10000) < millis()) break;
    // }
    Serial.print("Connecting");
    if (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
        WiFi.begin(ssid, password);
    }
    else {
        Serial.println("Connection failed");
        return;
    }
    Serial.println("\nConnected!");
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

    if(psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        delay(1000);
        ESP.restart();
    }

    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_QQVGA);

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setMaxRetry(firebaseData, 3);
    Firebase.setMaxErrorQueue(firebaseData, 30);
    Firebase.enableClassicRequest(firebaseData, true);

    String image_string;
    String photo_path = "/CAMERA/IMAGE";
    String flashpath = "/CAMERA/FLASH";
    String trigger_path = "/CAMERA/TRIGGER";
    bool use_flash = false;

    while(true) {
        Firebase.getBool(firebaseData, flashpath);
        use_flash = firebaseData.boolData() ? HIGH : LOW;
        Firebase.getBool(firebaseData, trigger_path);

        if (firebaseData.boolData()) { // if triggered then take pic and upload
            digitalWrite(FLASH, use_flash);
            
            image_string = Photo2Base64();

            Firebase.pushString(firebaseData, photo_path, image_string);
            Firebase.setBool(firebaseData, trigger_path, false);
        }
        digitalWrite(FLASH, LOW);
        delay(30);
    }
}

void loop() {
    delay(10000);
}
