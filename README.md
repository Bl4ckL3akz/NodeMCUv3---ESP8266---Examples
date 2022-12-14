# NodeMCUv3---ESP8266---Examples
A collection of Arduino -Codeing Stuff for the NodeMCUv3 - ESP8266 ,in SOFT_AP-mode and STATIOn_mode

*** This applications-sourcefile is located at : src\ ***

''' Device: NodeMCUv3 - ESP8266 '''
''' Codeing-language: VSCode - PlatformIO - Arduino IDE'''
''' This project supports  extensions like Adafruit_OLED-Display (ssd1306)'''
'''Author:BlackLeakz'''
'''Version: 0.1a'''
''' .ib-deps for PlatformIO: 
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
  '''
  
  lip-deps for Arduino IDE
  '''
  #include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
'''

'''DESCRIPTION: This WebServer runs in WiFi.Station-mode (STA_IF) , which need to connect to a hotspot or Soft_AP
Please edit the main.cpp file and replace your WiFi-credentials'''
