#include<Arduino.h>
#include"FirebaseESP8266.h"
#include "ESP8266WiFi.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>

const char* HOST = "aura-iot-c6bc8.firebaseio.com";
const char* SECRET = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";
const char* SSID = "INFINITUM0AEE_2.4";
const char* KEY = "7909283227";
FirebaseData firebaseData;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const int BT = D0;
const int ports[3] = {D5,D6,D7};

Servo servo;

bool estaCerca;

int val;

void setup(){
  pinMode(BT, INPUT);

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);


  Serial.begin(9600);
  WiFi.begin(SSID, KEY);
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
    delay(800);
  }
  Serial.println("Connected");
  Firebase.begin(HOST, SECRET);
  Serial.println("Firebase online");
  Firebase.reconnectWiFi(true);  
  Firebase.enableClassicRequest(firebaseData, true);


  servo.attach(5);
  servo.write(0);

  estaCerca = false;

  timeClient.begin();
  timeClient.setTimeOffset(-21600);
}

void loop(){
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm -> tm_mday;
  int currentMonth = ptm -> tm_mon + 1;
  int currentYear = ptm ->tm_year + 1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "/" + formattedTime;

  if (digitalRead(BT) == LOW){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);
  }
  else if (digitalRead(BT) == HIGH){
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
  }

  if (!estaCerca && digitalRead(BT) == HIGH){
    Firebase.setBool(firebaseData, "/Cerrojo/Proximidad", true);
    Firebase.pushString(firebaseData, "/Cerrojo/Storage", currentDate + ", Open");
    estaCerca = true;
  }
  else if (estaCerca && digitalRead(BT) == LOW){
    Firebase.setBool(firebaseData, "/Cerrojo/Proximidad", false);
    Firebase.pushString(firebaseData, "/Cerrojo/Storage", currentDate + ", Close");
    estaCerca = false;
  }

  Firebase.getBool(firebaseData, "/Cerrojo/Proximidad");
  val = firebaseData.boolData();
  if (val){
    servo.write(90);
  }
  else{
    servo.write(0);
  }
}