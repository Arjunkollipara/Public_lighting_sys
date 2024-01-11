#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ThingSpeak.h>

const char* wifiSSID = "NETWORKSSID";
const char* wifiPassword = "NETWORK PASSWORD";

const char* apSSID = "NodeMCU";
const char* apPassword = "SET A NEW PASSWORD HERE";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

uint8_t LED1pin = D0;
uint8_t LED2pin = D1;
uint8_t LED3pin = D2;

bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;

int led1 = D0; // GPIO 16 (D0) on NodeMCU
int led2 = D1; // GPIO 5 (D1) on NodeMCU
int led3 = D2; // GPIO 4 (D2) on NodeMCU

int ir1 = D5; // GPIO 14 (D5) on NodeMCU
int ir2 = D6; // GPIO 12 (D6) on NodeMCU
int ir3 = D7; // GPIO 13 (D7) on NodeMCU

WiFiClient client;

unsigned long myChannelNumber = 2229668;
const char* myWriteAPIKey = "CPWOZ4VXGVZEVS7U";

void setup() {
  Serial.begin(115200);
  WiFi.begin(wifiSSID, wifiPassword);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);

  ThingSpeak.begin(client);

  WiFi.softAP(apSSID, apPassword);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/ledon1", handle_led1on);
  server.on("/ledoff1", handle_led1off);
  server.on("/ledon2", handle_led2on);
  server.on("/ledoff2", handle_led2off);
  server.on("/ledon3", handle_led3on);
  server.on("/ledoff3", handle_led3off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  int val1 = digitalRead(ir1);
  int val2 = digitalRead(ir2);
  int val3 = digitalRead(ir3);
  int val4 = digitalRead(led1);
  int val5 = digitalRead(led2);
  int val6 = digitalRead(led3);

  ThingSpeak.writeField(myChannelNumber, 1, val1, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, val2, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, val3, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, val4, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 5, val5, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 6, val6, myWriteAPIKey);
}

// LED control and web server handler functions...
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status));
}

void handle_led1on() {
  LED1status = HIGH;
  digitalWrite(led1, HIGH);
  Serial.println("LED1 Status: ON");
  server.send(200, "text/html", SendHTML(true, LED2status, LED3status));
}

void handle_led1off() {
  LED1status = LOW;
  digitalWrite(led1, LOW);
  Serial.println("LED1 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status, LED3status));
}

void handle_led2on() {
  LED2status = HIGH;
  digitalWrite(led2, HIGH);
  Serial.println("LED2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true, LED3status));
}

void handle_led2off() {
  LED2status = LOW;
  digitalWrite(led2, LOW);
  Serial.println("LED2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false, LED3status));
}

void handle_led3on() {
  LED3status = HIGH;
  digitalWrite(led3, HIGH);
  Serial.println("LED3 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, true));
}

void handle_led3off() {
  LED3status = LOW;
  digitalWrite(led3, LOW);
  Serial.println("LED3 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
//HERE THE BELOW HTML CODE IS TO CREATE A INTERFACE WHEN WE CONNECT TO THE NODE MCU'S NETWORK AND GO TO THAT IP ADDRESS THERE THIS HTML WILL CREATE A INTERFACE TO CONTROL THE LED LIGHTS AND ALSO KNOW THEIR STATUS OVER THERE
String SendHTML(bool led1stat, bool led2stat, bool led3stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

  if (led1stat) {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/ledoff1\">OFF</a>\n";
  } else {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon1\">ON</a>\n";
  }

  if (led2stat) {
    ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/ledoff2\">OFF</a>\n";
  } else {
    ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/ledon2\">ON</a>\n";
  }

  if (led3stat) {
    ptr += "<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/ledoff3\">OFF</a>\n";
  } else {
    ptr += "<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/ledon3\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}