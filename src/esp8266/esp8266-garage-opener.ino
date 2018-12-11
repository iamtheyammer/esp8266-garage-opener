#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// modify settings below

// don't change unless you really need to
#define RELAY_PIN D6

// wifi ssid/password
string ssid = "mySSID"
string pass = "mywifipassword"

// if your relay needs negative to flip, set this to 1. otherwise (your relay flips with a positive current), set it to 0.
int relayReversePolarity = 1;

// don't modify below unless you have some knowledge of Arduino/C++

const int led = 2;
const String rootHTML = "<!doctypehtml><script>function pressGarageButton(){var e=new XMLHttpRequest;e.addEventListener(\"load\",onGaragePressSuccess),e.open(\"GET\",\"pressButton\"),e.send(),onGaragePress()}function onGaragePress(){document.getElementById(\"status\").innerHTML=\"Pressed, waiting for response...\"}function onGaragePressSuccess(){document.getElementById(\"status\").innerHTML=\"Successfully pressed the garage button.\"}</script><h2>Garage</h2><p id=status>Ready</p><button onclick=pressGarageButton() type=button>Press the garage button</button><br><br><p>Direct press link: /pressButton";
const String buttonPressResponse = "{\"status\":\"OK\"}";

ESP8266WebServer server(80);


// WEB SERVER HANDLES BELOW
void handleNotFound() {
  digitalWrite(led, 0);
  //  String message = "File Not Found\n\n";
  //  message += "URI: ";
  //  message += server.uri();
  //  message += "\nMethod: ";
  //  message += (server.method() == HTTP_GET)?"GET":"POST";
  //  message += "\nArguments: ";
  //  message += server.args();
  //  message += "\n";
  //  for (uint8_t i=0; i<server.args(); i++){
  //    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  //  }

  server.send(404, "application/json", "{\"status\":\"404 not found\"}");
  Serial.println("Handling 404...");
  digitalWrite(led, 1);
}
void handleRoot() {
  digitalWrite(led, 0);
  Serial.println("Handling root...");
  server.send(200, "text/html", rootHTML);
  digitalWrite(led, 1);
}
void handlePressButton() { // flip relay and respond with buttonPressResponse
  digitalWrite(led, 0);
  digitalWrite(RELAY_PIN, (relayReversePolarity == 0 ? HIGH : LOW)); // if relayReversePolarity is 1, flip to 0, vise versa.
  delay(100);
  digitalWrite(RELAY_PIN, (relayReversePolarity == 1 ? HIGH : LOW));
  server.send(200, "application/json", buttonPressResponse);
  digitalWrite(led, 1);
  //Serial.println(body);
}

// END WEB SERVER HANDLES

void connectToWiFi() {
  //WiFi.softAP(setupSSID);
  //WiFi.config(new IPAddress(10, 1, 20, 210), new IPAddress(10, 1, 20, 1), new IPAddress(10, 1, 20, 1), new IPAddress(255, 255, 255, 0));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("...");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void error() { // this function will NEVER return. run this to blink the LED infinitely, indicating an error.
  while (1 == 1) {
    digitalWrite(2, HIGH);
    delay(85);
    digitalWrite(2, LOW);
    delay(85);
    Serial.println("error!!");
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(2, 1); // of course, this is reversed!
  Serial.begin(9600);
  delay(100);
  //strcat(wifi_ssid_private, "SSID1234");
  //strcat(wifi_password_private, "PW1234");

  connectToWiFi(); // connects to WiFi

  //Serial.println(readEEPROM(0,106,test));
  if (MDNS.begin("garage")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/pressButton", handlePressButton);
  server.onNotFound(handleNotFound);
  server.begin();
  digitalWrite(RELAY_PIN, 0);
  Serial.print("Ready!");
  //  writeEEPROM(0,32,wifi_ssid_private);//32 byte max length
  //  writeEEPROM(32,32, wifi_password_private);//32 byte max length
  //  /*85 byte saved in total?*/
  //  Serial.println("everything saved...");
  //  readEEPROM(0,32,wifi_ssid_private);
  //  readEEPROM(32,32,wifi_password_private);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
