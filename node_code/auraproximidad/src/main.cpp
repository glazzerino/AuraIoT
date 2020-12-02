#include <Arduino.h>
#include"FirebaseESP8266.h"
#include "ESP8266WiFi.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string>

const char* HOST = "aura-iot-c6bc8.firebaseio.com";
const char* SECRET = "nUIJvAcFmcmfJEPpwlFXdTwmLjRjPtFUBbzFfLfk";
const char* SSID = "FMM10";
const char* KEY = "42521675";
FirebaseData firebaseData;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


#define PIN_TRIG D1
#define PIN_ECHO D0
#define MOTOR_A D5
#define MOTOR_B D6

float tiempo;
float distancia;
int door = 0;
int lock = 0;


void setup() {
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

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  analogWrite(MOTOR_A, 0);
  analogWrite(MOTOR_B, 0);

  timeClient.begin();
  // GMT +6 = 21600
  timeClient.setTimeOffset(-21600);
}

void loop() {
  
  timeClient.update();

  String formattedTime = timeClient.getFormattedTime();  
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "/" + formattedTime;

  digitalWrite(PIN_TRIG, LOW);  
  delayMicroseconds(4);
  digitalWrite(PIN_TRIG, HIGH);  
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
   
  tiempo = pulseIn(PIN_ECHO, HIGH);
  distancia = tiempo/58.3;
  Firebase.setInt(firebaseData, "/GARAGE_DOOR/US_SENSOR_v", distancia);


  Firebase.getInt(firebaseData, "/GARAGE_DOOR/lock");
  lock = firebaseData.intData();
  Firebase.getString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS");
  String temp;
  temp = firebaseData.stringData();
  if(temp == "Closed"){
    door = 0;
  }else{
    door = 1;
  }

  if (lock == 1) {
    if (door == 1){

      analogWrite(MOTOR_B, 1500);
      Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Closing");

      delay(1000);

      analogWrite(MOTOR_B, 0);

      door = 0;
      Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Closed");
      Firebase.pushString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS_log", currentDate + ", Closed");
      delay(750);
    }
  } else {
    if (distancia < 20 && !(distancia > 20)){
    Firebase.setInt(firebaseData, "/GARAGE_DOOR/US_SENSOR", 1);

      if (door == 0){
        analogWrite(MOTOR_A, 2500);
        Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Opening");
      

        delay(750);

        analogWrite(MOTOR_A, 0);

        door = 1;
        Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Opened");
        Firebase.pushString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS_log", currentDate + ", Opened");

        delay(3000);
      }
    
    } else {
      Firebase.setInt(firebaseData, "/GARAGE_DOOR/US_SENSOR", 0);

      if (door == 1){
        analogWrite(MOTOR_B, 1250);
        Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Closing");

        delay(1000);

        analogWrite(MOTOR_B, 0);

        door = 0;
        Firebase.setString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS", "Closed");
        Firebase.pushString(firebaseData, "/GARAGE_DOOR/DOOR_STATUS_log", currentDate + ", Closed");
        delay(750);
      }
    } 
   delay(750);
  }

  
}