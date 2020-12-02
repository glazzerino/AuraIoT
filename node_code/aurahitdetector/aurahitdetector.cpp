#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <NTPClient.h>
#include <WiFiUdp.h>  
#include "ESP8266WiFi.h"
#include <string>

const char* HOST = "aura-iot-c6bc8.firebaseio.com"; 
const char* SECRET = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";
const char* SSID = "Internet-de-la-NASA";
const char* KEY = "Ch4ll3ng3r_86";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
FirebaseData firebaseData;

#define vibration  D1
#define led  D7
int val;
int contExterno = 0;
int cont = 0;
bool verif = false;

void setup() {  
  Serial.begin(9600);
  WiFi.begin(SSID, KEY);
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(800);
  }
  Serial.println("");
  Serial.println("Connected");
  Firebase.begin(HOST, SECRET);
  Serial.println("Firebase online");
  Firebase.reconnectWiFi(true);
  pinMode(led, OUTPUT);
  pinMode(vibration, INPUT);
  Firebase.enableClassicRequest(firebaseData, true);
  timeClient.begin();
  timeClient.setTimeOffset(-21600);
}

void loop() {
  Firebase.getBool(firebaseData, "/HitDetector/Alarm");
  verif = firebaseData.boolData();
  if(!verif){
    val = digitalRead (vibration);
    Serial.println(val);
    if(val == HIGH){
      timeClient.update();
      String formattedTime = timeClient.getFormattedTime();
      unsigned long epochTime = timeClient.getEpochTime();
      struct tm *ptm = gmtime ((time_t *)&epochTime);
      int monthDay = ptm->tm_mday;
      int currentMonth = ptm->tm_mon+1;
      int currentYear = ptm->tm_year+1900;
      String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "/" + formattedTime;
      Firebase.setBool(firebaseData, "/HitDetector/Alarm", true);
      Firebase.pushString(firebaseData,"/HitDetector/Alarm_Log", currentDate); 
      digitalWrite(led, HIGH);
      Firebase.getBool(firebaseData, "/HitDetector/Alarm");
      verif = firebaseData.boolData();
      cont++;
      contExterno++;
      delay(500);
    }else{
      digitalWrite(led, LOW);
      contExterno = 0;
      delay(1500);
    }
  }else{
    if(contExterno == 0){
      timeClient.update();
      String formattedTime = timeClient.getFormattedTime();
      unsigned long epochTime = timeClient.getEpochTime();
      struct tm *ptm = gmtime ((time_t *)&epochTime);
      int monthDay = ptm->tm_mday;
      int currentMonth = ptm->tm_mon+1;
      int currentYear = ptm->tm_year+1900;
      String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "/" + formattedTime;
      Firebase.pushString(firebaseData,"/HitDetector/Alarm_Log", currentDate);
      contExterno++; 
    }
    if(cont%2 == 0){
      digitalWrite(led, HIGH);
      cont++;
    }else{
      digitalWrite(led, LOW);
      cont++;
    }
    delay(500);
  }  
}