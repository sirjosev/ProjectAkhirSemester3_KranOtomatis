
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt/Firebase-ESP8266
 * 
 * Copyright (c) 2022 mobizt
 *
*/

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#include "DHTesp.h"
#include <IrSignal.h>
#include <HCSR04.h>
#include <IrReceiverPoll.h>
#endif

#ifdef ESP8266
static constexpr pin_t RECEIVE_PIN = D5;
#else
static constexpr pin_t RECEIVE_PIN = D5;
#endif

static constexpr size_t BUFFERSIZE = 200UL;
static constexpr uint32_t BAUD = 115200UL;

IrReceiver *receiver;



//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "hai"
#define WIFI_PASSWORD "12345678"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBHKHzeScLmMbbuLy-zfLvdfWp4dRJn_5Y"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://kran-otomatis-d8942-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "josev.payon1@gmail.com"
#define USER_PASSWORD "Payon2022"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


Servo myservo;
DHTesp dht;

HCSR04 distanceSensor(D8, D7);  // Initialize sensor that uses digital pins 13 and 12.

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  dht.setup(D4, DHTesp::DHT11);

  receiver = new IrReceiverPoll(BUFFERSIZE, RECEIVE_PIN);
    Serial.print(F("Listening on pin "));
    Serial.println(receiver->getPin(), DEC);

}

  void loop()
  {
    int count = 0;
    float sudut_servo = 90;

    // int kelembapan = dht.getHumidity();
    // Firebase.setInt(fbdo, F("kelembapan"), kelembapan);

    float kelembapan = dht.getHumidity();
    kelembapan = kelembapan * 100; // Konversi ke persen jika perlu
    Firebase.setInt(fbdo, F("kelembapan"), (int)kelembapan);

    int ultra = distanceSensor.dist();
    Serial.print(ultra);
    Firebase.setInt(fbdo,F("ketinggian_air"), ultra);

    receiver->receive(); // combines enable, loop, disable

    if (receiver->isEmpty())
     {Serial.println(F("timeout"));}
    else {
      receiver->dump(Serial);
      Serial.println();
    }

      // Baca nilai dari kran_status di Firebase Realtime Database
    if (Firebase.getString(fbdo, F("kran_status"))) {
      String kranStatus = fbdo.stringData();


      // Jika kranStatus adalah "off", ubah sudut_servo menjadi 90
      // Jika kranStatus adalah "off", ubah sudut_servo menjadi 90
    if (kranStatus == "off") {
      sudut_servo = 90;
    }
    // Jika kranStatus adalah "on", ubah sudut_servo menjadi 180
    else if (kranStatus == "on") {
      sudut_servo = 180;
    }

    // Update sudut_servo dan kran_status di Firebase Realtime Database
    Firebase.setInt(fbdo, F("derajat_kran"), sudut_servo);
    Firebase.setString(fbdo, F("kran_status"), kranStatus);

    delay(100);
    }}


