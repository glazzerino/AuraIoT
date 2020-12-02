#include <Arduino.h>
#include "FirebaseESP8266.h"
#include "ESP8266WiFi.h"
#include <string>

const char* HOST = "aura-iot-c6bc8.firebaseio.com";
const char* SECRET = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";
const char* SSID = "INFINITUMEE87_2.4";
const char* KEY = "2035703114";
FirebaseData firebaseData;
const int ports[1] = {D1};
bool status[1] = {false};

void setup(){
  Serial.begin(9600);
    WiFi.begin(SSID, KEY);
    while ( WiFi.status() != WL_CONNECTED) {
        Serial.println("Connecting...");
        delay(800);
    }
    Serial.println("Connected");
    Firebase.begin(HOST, SECRET);
    Serial.println("Firebase online");
    Firebase.reconnectWiFi(true);
    pinMode(D1, OUTPUT);
    Firebase.enableClassicRequest(firebaseData, true);
}

void loop(){
  Firebase.getBool(firebaseData, "/Buzzer/Activate");
  bool act = firebaseData.boolData();
  if(act == true){
    Firebase.getBool(firebaseData, "/HitDetector/Alarm");
    bool est = firebaseData.boolData(); 
    if(est == true){
      Firebase.setString(firebaseData, "/Buzzer/State", "On");
      tone(D1, 420, 500);
      Firebase.setBool(firebaseData, "/CAMERA/TRIGGER", true);
      delay(1000);
    }
    else{
      Firebase.setString(firebaseData, "/Buzzer/State", "Off");
    }
  }
  else{
    Firebase.setString(firebaseData, "/Buzzer/State", "Off");
  }
}
