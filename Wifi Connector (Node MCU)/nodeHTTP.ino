/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <BlynkSimpleEsp8266.h>

#include <SoftwareSerial.h>

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;
HTTPClient http;

//rx tx
//SoftwareSerial tiva(D6,D5);

void ICACHE_RAM_ATTR readTiva();
void ICACHE_RAM_ATTR readTiva2();

int redScore = 0;
int blueScore = 0;
boolean isMatchRunning = false;

boolean newPoint = 0;

long last_t;

void setup() {

  Serial.begin(115200);

  pinMode(5, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(5), readTiva2, RISING);

  pinMode(13, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(13), readTiva, FALLING);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  Blynk.begin("Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg", "Mayar", "p1234567", "blynk-cloud.com");
  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Mayar", "p1234567");

}

void setValue(String pin, int value){
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    if (http.begin(client, "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/update/" + pin + "?value=" + value)) {  // HTTP
        // start connection and send HTTP header
        int httpCode = http.GET();
  
            //Serial.println(httpCode);
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          Serial.println(httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            //Serial.println(payload);
            newPoint = false;
          } 
          else {
            //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
  
  //      http.end();
      }
      else {
        //Serial.printf("[HTTP} Unable to connect\n");
      }
  }
  }
}

void getValue(String pin) {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    if (http.begin(client, "http://blynk-cloud.com/Twl7IpLaed9RZPZkiSJUEi-hM03Ju1bg/get/" + pin)) {  // HTTP
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            //Serial.println(payload);
            int val = 0;
            if(payload[3] == '"'){
                val = payload[2] - 48;
            }
            else{
              val = (payload[2] - 48)*10 + (payload[3] - 48);
            }
              
            
            if(pin == "v0"){
              redScore = val;
            }
            else if(pin == "v1"){
              blueScore = val;
            }
            else if(pin == "v2"){
              isMatchRunning = val;
            }
          }
        }
        else {
          //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
  
  //      http.end();
      }
      else {
        //Serial.printf("[HTTP} Unable to connect\n");
      }
  }
}

void readTiva(){
  long nt = millis();
  Serial.println("ISR");
  if(nt - last_t > 1000){ 
    Serial.println("inside");
    if(isMatchRunning){
      redScore++;
      Serial.println(redScore);
      newPoint= true;  
    }
  }
  last_t = millis();
}

void readTiva2(){
  long nt = millis();
  Serial.println("ISR2");
  if(nt - last_t > 1000){ 
    Serial.println("inside");
    if(isMatchRunning){ 
      blueScore++;
      Serial.println(blueScore);
      newPoint = true;
    }
  }
  last_t = millis();
}

void loop() {
  getValue("v2");
  if(!newPoint){
      getValue("v0");
      getValue("v1");
  }
  if(isMatchRunning && newPoint) {
    setValue("v0", redScore);
    setValue("v1", blueScore); 
  }

//  Serial.println(tiva.read());

  Blynk.run();

  delay(1000);
}
