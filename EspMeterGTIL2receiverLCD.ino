// EspMeterGTIL2receiverLCD.ino

/**
   StreamHTTPClient.ino
    Created on: 24.05.2015
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
//_________________________________________________________

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
 

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
//int  http.getString() ;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);                         // your connection router
  WiFiMulti.addAP("XX", "XX");                 // YOUR WIRELESS NETWORK NAME ,YOUR PASSWORD

   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done
 
  display.display();
  delay(2000);
 
  // Clear the buffer.
  display.clearDisplay();
}

void loop() {
  // wait for WiFi connection


/**
   StreamHTTPClient.ino
     
    delay(2000);
    Serial.print("[HTTP] connection closed or file end.1\n");
    delay(2000);
void controllo();
Serial.print("[HTTP] connection closed or file end.2\n");
delay(2000);
void controllo02();
Serial.print("[HTTP] connection closed or file end.3\n");
delay(2000);
}

void controllo(){
    Created on: 24.05.2015
*/




   if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;  // must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

    Serial.print("[HTTP] begin...\n");

    // configure server and url
    http.begin(client, "http://192.168.23.40:80/temperature");
    // http.begin(client, "192.168.23.40", 80, "/HTTP/connection.html");

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get length of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[20] = { 0 };
        int buff2;
//uint8_t buff[20] = buff2;
#if 0
        // with API
        Serial.println(http.getString());
        display.display();
  delay(100);
 
  // Clear the buffer.
  display.clearDisplay();
        display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(len);
  display.display();
 // delay(2000);
  display.clearDisplay();
#else
        // or "by hand"

        // get tcp stream
        WiFiClient* stream = &client;

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // read up to 128 byte
          int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));
          Serial.printf("readBytes: %d\n", c);
          if (!c) { Serial.println("read timeout"); }

          // write it to Serial
          Serial.write(buff, c);
       //   Serial.print(buff, c);

  display.display();
//  delay(100);
 
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Consumo");
  display.println("");
  display.setTextSize(2);
    display.print((char *)buff);
    display.println("W");
  display.display();
 // delay(2000);
 // display.clearDisplay();




          if (len > 0) { len -= c; }
        }
#endif

        Serial.println();
        Serial.print("[HTTP] connection closed or file end.\n");
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

}
delay(1200);
//String c=int;
//int = (http.getString()) ;
//if ((http.getString()) >500) {   // OVER 500W switch off load connected to ESPURNA
// http.begin("http://192.168.XX.XX/api/relay/1?apikey=XXXXXXXXXXXXXXXX&value=0"); //HTTP  REPLACE IP relay AND APYKEY control ESPURNA for switch off if load is OVER 500W
//                                               int httpCode = http.GET();
//                                                http.end();
//                                                delay(3000);
//                                  }              
}
