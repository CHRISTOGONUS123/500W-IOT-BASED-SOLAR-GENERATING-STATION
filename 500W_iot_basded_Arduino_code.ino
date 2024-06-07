#include <LiquidCrystal_I2C.h>
// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
#include <Wire.h>
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Airtel_PocketMifi_CAF5"
#define WIFI_PASSWORD "W8ca227f34"

// Insert Firebase project API Key
#define API_KEY "AIzaSyA8DExHQgu5jbVUc_XJu6_aG6HjQzJMhZg"

// Insert RTDB URLefine the RTDB URL

#define DATABASE_URL "farm-b4ae4-default-rtdb.firebaseio.com/" 

// Define relay pin and LED pin
//#define relay 23

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

//LCD
const int voltSensing = 12;  //GPIO12-ADC15  
int voltSensingVal = 0;  
int currentSensor = 34;  //GPIO35-ADC7 
double battVolt = 36; //GPIO39-ADC0
double BATTERY = 0;
const int relay = 25; //GPIO25 
int AC_Voltage = 39;  //GPIO39-ADC3
int AC_VOLT = 0;

#include "ACS712.h"
ACS712  ACS(currentSensor, 3.3, 4095, 100);
float mA, CURRENT; //amp

//const float ACS712_Sensitivity = 0.185; // Sensitivity for ACS712-05B

void setup() {
  lcd.init();                        
  lcd.backlight();
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);  // Ensure relay is off at start
  //analogReadResolution(12); // Set ADC resolution to 12 bits
  
  lcd.setCursor(1, 0); 
  lcd.print("DEVELOPMENT OF AN");
  lcd.setCursor(2, 1);
  lcd.print("IOT BASED SOLAR");  
  lcd.setCursor(1, 2);  
  lcd.print("INVERTER MONITORING"); 
  lcd.setCursor(6, 3);
  lcd.print("SYSTEM"); 
  delay(3000);
  
  ACS.autoMidPoint();
  pinMode(currentSensor, INPUT);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the api key (required) 
  config.api_key = API_KEY;

  // Assign the RTDB URL (required) 
  config.database_url = DATABASE_URL;

  // Sign up 
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long running token generation task 
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

/*
void checkCurrent(){
  //For Current Sensor 
    mA = ACS.mA_AC();
    CURRENT = abs((mA / 1000)); //amp
     if(CURRENT < 0.08){
      CURRENT * 0.0;
     }
     if(CURRENT >= 0.01){
      CURRENT - 0.10;
     }     
}*/

void loop() {
  //This is to check the AC VOLTAGE
    AC_VOLT = analogRead(AC_Voltage); 
    AC_VOLT = (AC_VOLT * 0.08835); //0.092586
  
    //This is to check if the battery is charging
    voltSensingVal = analogRead(voltSensing); 
    voltSensingVal = (voltSensingVal * 0.001221); // 
    
    //For Current Sensor 
      mA = ACS.mA_AC();
      CURRENT = abs((mA / 1000)); //amp
      CURRENT = CURRENT - 0.10;
      
//  int currentSensorValue = analogRead(currentSensor);
//  float CURRENT = (currentSensorValue - 512) * (3.3 / 1024.0) / ACS712_Sensitivity;
   
    //For battery voltage 
    BATTERY = analogRead(battVolt);     
    BATTERY = (BATTERY * 0.00442612); // 5/4049=0.001221
    
    //Wattage
    double WATTAGE = AC_VOLT * CURRENT;

    //Display on serial monitor and App
    //lcd.clear();
//    if(voltSensingVal > 600){
//      Serial.println("Battery Charging"); 
//      lcd.setCursor(0,0);
//      lcd.print("Battery Charging");
//      lcd.clear();
//    }
//    else{
//      Serial.println("Battery Not Charging");
//      lcd.setCursor(0,0);
//      lcd.print("Battery Not Charging");
//      lcd.clear();
//    }
    lcd.clear();
    Serial.print("AC_Volt = "); Serial.print(AC_VOLT); Serial.println("V");
    Serial.print("AC_Current = "); Serial.print(CURRENT); Serial.println("A");
    Serial.print("Batt_Volt = "); Serial.print(BATTERY,2); Serial.println("V");
    Serial.print("Wattage = "); Serial.print(WATTAGE,2); Serial.println("W");
    
    lcd.setCursor(0,0); lcd.print("AC_Volt = "); lcd.setCursor(10,0); lcd.print(AC_VOLT); lcd.setCursor(18,0); lcd.print("V");
    lcd.setCursor(0,1); lcd.print("Current = "); lcd.setCursor(10,1); lcd.print(CURRENT); lcd.setCursor(18,1); lcd.print("A");
    lcd.setCursor(0,2); lcd.print("Batt_Volt = "); lcd.setCursor(12,2); lcd.print(BATTERY,2); lcd.setCursor(18,2); lcd.print("V");
    lcd.setCursor(0,3); lcd.print("Wattage = "); lcd.setCursor(10,3); lcd.print(WATTAGE,2); lcd.setCursor(18,3); lcd.print("W");
    //lcd.clear();
    
    if(Firebase.RTDB.setInt(&fbdo, "AC_VOLT", AC_VOLT)){
      Serial.println("PASSED");
    }  
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "CURRENT", CURRENT)){
      Serial.println("PASSED");
    }

  if (Firebase.RTDB.setFloat(&fbdo, "BATTERY", BATTERY)){
      Serial.println("PASSED");
    }
  if (Firebase.RTDB.setFloat(&fbdo, "WATTAGE", WATTAGE)){
      Serial.println("PASSED");
    }

// Read relay state from Firebase
  Firebase.RTDB.getString(&fbdo, "/RELAY");
  String relayState = fbdo.stringData();
  if (relayState == "1"){
    digitalWrite(relay, HIGH); // Energize relay
    //digitalWrite(LED_PIN, HIGH);   // Turn on LED
    Serial.println("Relay ON");
  } else if (relayState == "0") {
    digitalWrite(relay, LOW);  // De-energize relay
    //digitalWrite(LED_PIN, LOW);    //Turn off LED
    Serial.println("Relay OFF");
  }
  delay(500); //Delay to prevent rapid polling
}
