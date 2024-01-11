#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ThingSpeak.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

WiFiClient client;

unsigned long myChannelNumber = YOURCHANNEL NUMBER;
const char* myWriteAPIKey = "WRITE API KEY";
const char* myReadAPIKey = "READ API KEY";

// Insert Firebase project API Key
#define API_KEY "AIzaSyBiGZ_IE6hdqD2QCPk-fE3CxCw0XzrFVwA"

// Insert RTDB URL
#define DATABASE_URL "https://test2-dd6f5-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int led_1_state = LOW;
int led_2_state = LOW;
int led_3_state = LOW;

int ir1 = D5; // GPIO 14 (D5) on NodeMCU
int led1 = D0; // GPIO 16 (D0) on NodeMCU

int ir2 = D6; // GPIO 12 (D6) on NodeMCU
int led2 = D1; // GPIO 5 (D1) on NodeMCU

int ir3 = D7; // GPIO 13 (D7) on NodeMCU
int led3 = D2; // GPIO 4 (D2) on NodeMCU

int ldr = A0;

int val = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key*/
  config.api_key = API_KEY;

  /* Assign the RTDB */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase authentication success.");
  }
  else {
    Serial.printf("Firebase authentication failed: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(ir1, INPUT);
  pinMode(led1, OUTPUT);

  pinMode(ir2, INPUT);
  pinMode(led2, OUTPUT);

  pinMode(ir3, INPUT);
  pinMode(led3, OUTPUT);

  ThingSpeak.begin(client);
}

void loop() {
  int val1 = digitalRead(ir1);
  int val2 = digitalRead(ir2);
  int val3 = digitalRead(ir3);
  int val4 = digitalRead(ldr);

  if (val1 == 1 && val4 == 0 && val2 == 1 && val3 == 1) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }
  else if (val1 == 1 && val4 == 1 && val2 == 1 && val3 == 1) {
    analogWrite(led1, 20);
    analogWrite(led2, 20);
    analogWrite(led3, 20);
  }

  if (val1 == 0 && val4 == 1) {
    analogWrite(led1, 500);
   // analogWrite(led2, 20);
   // analogWrite(led3, 20);
  }
  if (val4 == 1 && val2 == 0) {
    analogWrite(led2, 500);
   // analogWrite(led1, 20);
    //analogWrite(led3, 20);
  }
  if (val4 == 1 && val3 == 0) {
    analogWrite(led3, 500);
    //analogWrite(led1, 20);
   // analogWrite(led2, 20);
  }

  int s1 = digitalRead(ir1);
  int s2 = digitalRead(ir2);
  int s3 = digitalRead(ir3);
  int s4 = digitalRead(ir3);
  int s5 = digitalRead(ir3);
  int s6 = digitalRead(ir3);
  s3 = !s3;

  val = analogRead(ldr);

  Serial.print(s1);
  Serial.print(":");
  Serial.print(s2);
  Serial.print(":");
  Serial.print(s3);
  Serial.print("  ");
  Serial.println(val);

  if (val < 800) {
    led_1_state = s1;
    led_2_state = s2;
    led_3_state = s3;
  } else {
    led_1_state = LOW;
    led_2_state = LOW;
    led_3_state = LOW;
  }

  digitalWrite(led1, led_1_state);
  digitalWrite(led2, led_2_state);
  digitalWrite(led3, led_3_state);

  // LED states to Firebase
  Firebase.RTDB.setInt(&fbdo, "/leds/led1", led_1_state);
  Firebase.RTDB.setInt(&fbdo, "/leds/led2", led_2_state);
  Firebase.RTDB.setInt(&fbdo, "/leds/led3", led_3_state);

  // sensor data to ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 1, val, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, s1, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, s2, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, s3, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 5, s4, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 6, s5, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 7, s6, myWriteAPIKey);

  delay(100); // Delay to prevent excessive data updates
}