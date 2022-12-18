//code copyright Cent0 by Github whit help Cattledog for Arduino forum
// Import required libraries
//#include <Arduino.h>      //
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

                                                                //      my code
#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3);
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
 
// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

//int buffer[40];

int ProdI;
int VDc;
int VoltAc;
 int   Watt;
 int   VDC;
 int   Temp;
 int   WattHome;
 
const byte startMarker = 0x94;
const byte messageLength = 21;          //18 
byte receivedBytes[messageLength + 1]; //include start marker
boolean newData = false;
boolean newData2 = false;
static boolean recvInProgress = false;
//                                                                   Set your access point network credentials
const char* ssid = "XXX";//   your wifi
const char* password = "XXX";//   your passwor

// Set your Static IP address
IPAddress local_IP(192, 168, xx, xx); //ip espdevice
// Set your Gateway IP address
IPAddress gateway(192, 168, xx, x); // gateway

IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional

//String WattHome 

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String readTemp() {
   return String(WattHome);
 }
String readHumi() {
  return String(ProdI);                // return String(bme.readHumidity());
}
String readdc() {
  return String(VDc);                // aggiunto;
}
String readPres() {
  return String(VoltAc);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(500000);   // 115200
  SSerial.begin(9600);
  Serial.println();
    // tolto messo nel loop
     // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open

  if (!WiFi.config(local_IP, gateway, subnet)) {    // , primaryDNS, secondaryDNS
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
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



//  WiFi.softAP(ssid, password);

 // IPAddress IP = WiFi.softAPIP();
 // Serial.print("AP IP address: ");
 // Serial.println(IP);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done
 
  display.display();
  delay(50);
}

void loop()
{
  recvBytesWithStartMarker();
 
  delay(10);

  showNewData();
 delay(10);
  
  if (newData2 == true)
 {
collectData();
//  printNewData();
  displayNewData();
  funzione();
  }
}

void recvBytesWithStartMarker()
{
newData2 = false;     // add for stable cycle loop LAST EDIT 17/12/2022
  static boolean recvInProgress = false;
  static int ndx = 0;
  byte rc;

  while (SSerial.available() > 0 && newData == false) 
  {
    rc = SSerial.read();

    if (recvInProgress == true)
    {
      receivedBytes[ndx] = rc;
    //  if (ndx == 8 and rc != 0x05)
    //  if ((ndx == 12 and rc != 0x04) and (ndx == 21 and rc != 0x34))
     if (ndx == 21 and rc != 0x34)
      {
  //__________________________________________________________________________________
       Serial.println("This just in false ");
    for (byte i = 0; i <= messageLength; i++)
    {
      Serial.print(receivedBytes[i], HEX);
      Serial.print(" ");
    }
//____________
        
        recvInProgress = false;
 //       newData = false;          // aggiunto _________________________
        ndx = 0;
      }
      else
        ndx++;
      if (ndx == messageLength + 1)
      {
        newData = true;
        
        recvInProgress = false;
        ndx = 0;
      }
    }
 //   else if (rc == startMarker || rc == 0x95 )
 //   else if ( rc == 0x95 )
      else if ((rc == 0x94)|| (rc == 0x95) || (rc == 0x96)  )
   // else if ( rc == 0x94 || rc == 0x95 || rc == 0x96 || rc == 0x93 )
    
    {
      recvInProgress = true;
      ndx = 0;  // save start marker
      receivedBytes[ndx] = rc;
      ndx++;
  // if (ndx == 1 and rc == 0x94 or rc == 0x95 && ndx == 13 and rc == 0x04)
  // if ((ndx == 12 and  rc != 0x04) and (ndx == 21 and rc != 0x34))
  //    {
  //    
  // recvInProgress = false;
      
        }
    }
  //__________________________________________________________________________________
 //       Serial.println("This just in false ");
 //   for (byte i = 0; i <= messageLength; i++)
 //   {
//      Serial.print(receivedBytes[i], HEX);
//      Serial.print(" ");
//    }
//__________________________________________________________________________________
   
  }
//}

void showNewData()
{
 // if ((newData == true) and (recvInProgress == true))
    if (newData == true) 
  {
    Serial.println("This just in ...OK ");
    for (byte i = 0; i <= messageLength; i++)
    {
      Serial.print(receivedBytes[i], HEX);
      Serial.print(" ");                        // IN LINEA
    }
     Serial.println(" ");
 
newData = false;
newData2 = true;
}
}

void collectData()
{
VoltAc = ((receivedBytes[1]*256+receivedBytes[2])/10);
ProdI =  ((receivedBytes[3]*256+receivedBytes[4])/10);
VDc =    ((receivedBytes[5]*256+receivedBytes[6])/8);
Temp =   ((receivedBytes[13]*256+receivedBytes[14])/24);
WattHome = (((int32_t)receivedBytes[15] << 24) |((int32_t)receivedBytes[16] <<16) |
             ((int32_t)receivedBytes[17] << 8 ) |(int32_t)receivedBytes[18])/10;
    }
void printNewData()
{
    Serial.println("_____________________");
    Serial.print("|Voltaggio AC|= ");
    VoltAc = ((receivedBytes[1]*256+receivedBytes[2])/10);
 //   if (VoltAc > 200 && VoltAc < 260) { 
    Serial.print(VoltAc,DEC);
    Serial.println(" V");
    

   // Serial.println("_____________________");
    Serial.println("|Produzione  |");
    Serial.print  ("|Istantanea  |= ");
    ProdI = ((receivedBytes[3]*256+receivedBytes[4])/10);
  //  ProdI = ((receivedBytes[4]*256+receivedBytes[5])/10);
    Serial.print(ProdI);
    Serial.println(" W");
  //  Serial.println("_____________________");
    Serial.print("|Voltaggio DC|= ");
    VDc =   ((receivedBytes[5]*256+receivedBytes[6])/8);
    Serial.println(VDc);
  //  Serial.println("_____________________");
  //  VDC  = (receivedBytes[6]*256+receivedBytes[7]);
 //   Serial.println(VDC);
 // Serial.println("_____________________");
  Serial.print("|Temperatura |= ");
  Temp = ((receivedBytes[13]*256+receivedBytes[14])/24);
  Serial.print(Temp);
   Serial.println("°C");
 // Serial.println("_____________________");
  Serial.println("|Consumo     |"); 
  Serial.print  ("|Istantaneo  |= ");
 // WattHome = ((receivedBytes[17]*256+receivedBytes[18])/10);
  WattHome = (((int32_t)receivedBytes[15] << 24) |((int32_t)receivedBytes[16] <<16) |
             ((int32_t)receivedBytes[17] << 8 ) |(int32_t)receivedBytes[18])/10;
  
  Serial.print(WattHome);
  Serial.println(" W");
  Serial.println("_____________________");
 //   Serial.println("This just in .please this work ??? please ? HOOOOOOOO.. ");
     
    
   
     Serial.println("INVERTER LOG");
}


void displayNewData()
 {

  display.display();  //  FA LAMPEGGIARE
  //delay(50);
 
  // Clear the buffer.
  display.clearDisplay();
 
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Casa:" );
  display.print(WattHome);
  display.println("W" );
  display.print("VAc :" );
  display.print(VoltAc);
  display.println("V" );
  display.print("Temp:");
  display.print(Temp);
  display.println("C" );
  display.print("VDc :");
  display.print(VDc);
  display.println("V" );
  display.println("---------" );
  display.print("Prod:");
  display.print(ProdI);
  display.println("W" );

}

void funzione()

{ 
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){                //______________________
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){                 //___________________
    request->send_P(200, "text/plain", readHumi().c_str());
  });
    server.on("/vdc", HTTP_GET, [](AsyncWebServerRequest *request){                 //___________________
    request->send_P(200, "text/plain", readdc().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());                            //____________________
  });
  server.begin(); 
  newData2 = false;
}
