#include <Arduino.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "BlackLeakz-Station";
const char* password = "1234567890";

int numberOfNetworks;
unsigned long lastAusgabe;
const unsigned long intervall = 5000;



AsyncWebServer server(80);



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BlackLeakz-OTA-Server& WiFi Scan</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
     head{
      background-color: #878787; 
      color: #00FF9E;
    }
    body{
      background-color: #878787; 
      color: #00FF9E;
    }
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <center><h2>BlackLeakz-OTA-Server&WiFi Scan</h2></center>
  <center><p3>OTA-Update: </p3></center><center><a class=\"button button-on\" href="/update">Update</a></center>
  <center><p3>Scan</p3></center><center><a class=\"button button-on\" href="/scan">Scan</a></center>
  <center><p2>WiFi Scan2: </p2></center><center><a class=\"button button-on\" href="/scan2">Scan</a></center>
  <center><p5>WiFi Info: </p5></center><center><a class=\"button button-on\" href="/api/wifi-info">NFO</a></center>
  <center><h5>Return to BlackLeakz-Repeater Config</h5></center><a class=\"button button-on\" href="/main"<Main</a></center>
  <center><h5>Return to BlackLeakz-Repeater Redirector</h5></center><a class=\"button button-on\" href="/main2"<Main2</a></center>
</body>
</html>
)rawliteral";


const char redirect[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BlackLeakz-OTA-Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="Refresh" content="5; url=http://192.168.4.1" />
</html>
)rawliteral";


const char redirectx[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BlackLeakz-OTA-Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="Refresh" content="5; url=http://192.168.4.102" />
</html>
)rawliteral";







void setup(void) {
  Serial.begin(115200); // Starts Serial communication with a baudrate of 115200
  Serial.println("ConsoleLog<>> started.");

  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(2000);  

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR)) // Initilaizes the OLED Display
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();  
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Starting...");
  display.display();
  display.setCursor(0,10);
  display.print("Device...");
  display.display();
  delay(1000);

  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");


  // Wait for connection
  int y = 0;
  int c = 0;
  display.clearDisplay();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.setCursor(y,c);
    display.print(".");
    y++;
    
  }

  Serial.println("");
  Serial.print("ConsoleLog<>> Connected to ");
  Serial.println(ssid);
  Serial.print("ConsoleLog<>> IP address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("IP-ADDR:");
  display.display();
  display.setCursor(0,10);
  display.println(WiFi.localIP());
  display.display();



  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("\nConsoleLog<>> request root-page.");
    request->send_P(200, "text/html", index_html);
  });

  server.on("/main", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("\nConsoleLog<>> Redirect  to config-page.");
    request->send_P(200, "text/html", redirect);
  });

  server.on("/main2", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("\nConsoleLog<>> Redirect  to extended2-page.");
    request->send_P(200, "text/html", redirectx);
  });

  server.on("/api/wifi-info", HTTP_GET, [](AsyncWebServerRequest *request) {
      AsyncResponseStream *response = request->beginResponseStream("application/json");
      DynamicJsonDocument json(1024);
      json["status"] = "ok";
      json["ssid"] = WiFi.SSID();
      json["ip"] = WiFi.localIP().toString();
      serializeJson(json, *response);
      request->send(response);
  });

 server.on("/scan2", HTTP_GET, [](AsyncWebServerRequest *request) {
  String content;

  Serial.println("ConsoleLog<>> scan start");

  int n = WiFi.scanNetworks();

  numberOfNetworks = n;  // für OLED-Anzeige

  Serial.println("ConsoleLog<>> scan done.");
  if (n == 0) {
    Serial.println("ConsoleLog<>> no networks found.");
    content += "ConsoleLog<>> no networks found.";
  } else {
    Serial.println("ConsoleLog<>> ");
    Serial.print(n);
    Serial.println(" networks found");
    content = "<!DOCTYPE html><html lang='en'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    content += "<head><title>ESP8266 Configuration Page</title></head>";
    content += "<body>";
    content += "<form method='post'><center><p><button type='submit' formaction='scan'>Scan</button></p>\n<p>";
    content += String("ConsoleLog<>> ") + String(n) + String(" networks found\n");
    content += "</p></center></form>";

    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      content += String("<p>") + String(i + 1) + String(": ") + String(WiFi.SSID(i)) + String(" (") + String(WiFi.RSSI(i)) + String(")</p>\n") ;
      delay(10);
    }
  }
  Serial.println("");
  content += "</body></html>";
    request->send(200, "text/html", content);
 });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "[";
    int n = WiFi.scanComplete();
    if(n == -2){
    WiFi.scanNetworks(true);
    } else if(n){
    for (int i = 0; i < n; ++i){
      if(i) json += ",";
      json += "{";
      json += "\"rssi\":"+String(WiFi.RSSI(i));
      json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
      json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
      json += ",\"channel\":"+String(WiFi.channel(i));
      json += ",\"secure\":"+String(WiFi.encryptionType(i));
      json += ",\"hidden\":"+String(WiFi.isHidden(i)?"true":"false");
      json += "}";

    }
    WiFi.scanDelete();
    if(WiFi.scanComplete() == -2){
      WiFi.scanNetworks(true);
    }
  }
  json += "]";
  request->send(200, "application/json", json);
  json = String();
  
});


  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(LED_BUILTIN, LOW);   
    request->send(200, "text/plain", "On");
  });
  
  
  
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();


  Serial.println("ConsoleLog<>> HTTP server started.");
}

void loop() {

  Serial.println("ConsoleLog<>> Led is blinking now. ");

  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);


  Serial.println("/nConsoleLog<>> WiFi Scan, print result to oled.");

  if (numberOfNetworks <= 0)
    {
      numberOfNetworks = WiFi.scanNetworks();
      Serial.print(numberOfNetworks);
      Serial.println(F(" gefundene Netzwerke"));
      }
      else if (millis() - lastAusgabe > intervall)
      {
        numberOfNetworks--;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Network-name: ");
        display.setCursor(0, 10);
        display.print(WiFi.SSID(numberOfNetworks));
        display.setCursor(0, 20);
        display.print("Signal strength: ");
        display.setCursor(0, 30);
        display.print(WiFi.RSSI(numberOfNetworks));
        display.display();
        Serial.print("Network name: ");
        Serial.println(WiFi.SSID(numberOfNetworks));
        Serial.print("Signal strength: ");
        Serial.println(WiFi.RSSI(numberOfNetworks));
        Serial.println("-----------------------");
        lastAusgabe = millis();
        }
  

}



