// Dave Demos 05/28/2023 thank you for news
// EspMeterGtil2new.cpp
// new testing for Transmitter with Simple CODE 
// the device transmitt Direct to smarphone or Tablet / pc
// if new Programmer have sugestion for increase stability code plese help!
// to nex release if possible add Mqtt for sending data to any server.
// github Cent0
// https://www.youtube.com/@s-max3277

// Thermistor resistance at 25 degrees C
#define THERMISTORNOMINAL 102000      
// Temperature for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 100000

#include <Arduino.h>
#include <ESP8266WiFi.h>

//#include <ESP8266HTTPClient.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h> 
//#include <ArduinoOTA.h> 
//#include <Hash.h>
//#include <ESPAsyncTCP.h>
// //#include "ESPAsyncWebServer.h"
//#include <ESPAsyncWebServer.h>

// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3); // RX (GPIO2), TX (GPIO3)

// SCL GPIO5
// SDA GPIO4

// #define OLED_RESET 0  // GPIO0
// Adafruit_SSD1306 display(OLED_RESET);

byte myData[300];

int16_t Load;

float VAC;
float Output;
float VDC;
float TempConv;
float TempF;
float Resistance;
float steinhart;

const char* ssid = "A.pc";
const char* password = "3000123458";

WiFiServer server(80);

String header;

IPAddress local_IP(192, 168, xx, xx); // IP address of ESP device
IPAddress gateway(192, 168, xx, xx); // IP of Gateway

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {

  pinMode(2, OUTPUT);    // Built in LED
  digitalWrite(2, HIGH); // Turn LED off
  
  Serial.begin(19200);
  SSerial.begin(9600);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("This WiFi client module MAC address is:");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);                      // Station Mode. WiFi Client Only, No AP
  WiFi.begin(ssid, password);
  IPAddress ip(192,168,xx,xx);
  IPAddress gateway(192,168,xx,xx);
  IPAddress subnet(255,255,255,0);
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
// Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
//  display.display();
//  delay(50);

}

void loop() {

byte n = SSerial.available();

  if (n >= 2) {

    byte y0 = SSerial.read();
    byte y1 = SSerial.read();

    unsigned int y = y0 << 8 | y1;

    if (y == 0x0707) { // Sync found

      digitalWrite(2, LOW); // Turn built in LED on as data is received into the buffer
      
      SSerial.readBytes(myData, 296);

      for (int i = 0; i < 296; i++)
 
      digitalWrite(2, HIGH); // Turn bult in LED off
        
      VAC = (myData[278]*256+myData[279])*0.1;
      Output = (myData[280]*256+myData[281])*0.1;
      VDC = (myData[282]*256+myData[283])*0.125;
      TempConv = (myData[290]*256+myData[291]);
      Load = (myData[294]*256+myData[295]);
      Load = Load * 0.1;


      // Convert the value to resistance
      TempConv = 1023 / TempConv - 1;
      TempConv = SERIESRESISTOR / TempConv;

      steinhart = TempConv / THERMISTORNOMINAL;         // (R/Ro)
      steinhart = log(steinhart);                       // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
      steinhart = 1.0 / steinhart;                      // Invert
      steinhart -= 273.15;                              // Convert absolute temp to C
      TempF = ((steinhart)*1.8)+32;                     // Convert C to F

        Serial.println("");
        Serial.print(F("Line Voltage: "));
        Serial.print(VAC, 1);
        Serial.println(F(" VAC"));

        Serial.print(F("Output: "));
        Serial.print(Output, 1);
        Serial.println(F(" W"));

        Serial.print(F("AC Load: "));
        Serial.print(Load);
        Serial.println(" W");

        Serial.print(F("Solar Voltage: "));
        Serial.print(VDC, 1);
        Serial.println(F(" VDC"));

        Serial.print(F("Thermistor resistance: ")); 
        Serial.print((TempConv) / 1000, 3);
        Serial.println(F(" K Ohms")); 

        Serial.print(F("Temperature: "));
        Serial.print(steinhart, 1);
        Serial.println(F(" C"));

        Serial.print(F("Temperature: "));
        Serial.print(TempF, 1);
        Serial.println(F(" F"));
        Serial.println("");

/*
  display.display();
 
  // Clear the buffer.
  display.clearDisplay();
 
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Line: " );
  display.print(VAC, 1);
  display.println(" VAC" );
  display.print("Outut: " );
  display.print(Output, 1);
  display.println(" W" );
  display.print("Load: ");
  display.print(Load, 1);
  display.println(" W" );
  display.print("VDC: ");
  display.print(VDC, 1);
  display.println(" VDC" );
  display.print("Temp: ");
  display.print(steinhart, 1);
  display.println(" C" );
*/

      }
    }

  // Listen for incoming clients  
  WiFiClient client = server.available();

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // Print a message out in the serial port
    String currentLine = "";                // Make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // Loop while the client is connected
      currentTime = millis();         
      if (client.available()) {             // If there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {  
                            
// If the byte is a newline character
// if the current line is blank, you got two newline characters in a row.
// that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {
            
// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
          //  client.println("Refresh: 9"); // refresh the page automatically every 5 sec
                        client.println("Refresh: 5"); // refresh the page automatically every 5 sec

            client.println();

// Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
// Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} </style></head>");
            
// Web Page Heading
            client.println("<body></h2><font color=blue><h1>GTIL2 Inverter Cent0</h2>");
            
// Display current readings 
            client.print(F("<p><h2><font size=4><font color=black>Volt AC: "));
            client.print(VAC, 1);
            client.print(F(" VAC"));
            client.println(F("</font></h2></p>"));

            client.print(F("<p><h2><font size=4><font color=black>Produzione: "));
            client.print(Output, 1);
            client.print(F(" Watts"));
            client.println(F("</font></h2></p>"));

            client.print(F("<p><h2><font size=4><font color=black>CONSUMO: "));
            client.print(Load);
            client.print(F(" Watts"));
            client.println(F("</font></h2></p>"));    

            client.print(F("<p><h2><font size=4><font color=black>Volt DC: "));
            client.print(VDC, 1);
            client.print(F(" VDC"));
            client.println(F("</font></h2></p>"));

            client.print(F("<p><h2><font size=4><font color=black>Inverter Temperature: "));
            client.print(steinhart, 1);
            client.print(F(" deg C"));
            client.println(F("</font></h2></p>"  ));                        
               
            
// The HTTP response ends with another blank line
            client.println();
            
// Break out of the while loop
            break;

// If you got a newline, then clear currentLine            
          } else {
            currentLine = "";
          }

// if you got anything else but a carriage return character, add it to the end of the currentLine
        } else if (c != '\r') {  
          currentLine += c;
        }
      }
    }
    
// Clear the header variable
    header = "";

// Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");

  }
}
