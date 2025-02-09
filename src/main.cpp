/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <Wire.h>                                   // library for I2C comunication
#include "SSD1306Wire.h"                      // library for display
SSD1306Wire display(0x3c, SDA, SCL);   // I2C address of display

int lives = 0;
const char* ssid     = "afina";
const char* password = "black_kirill";

WiFiServer server(80);
String header;
String output5State = "off";
String output4State = "off";

const int output5 = 5;
const int output4 = 4;


void setup() {
  Serial.begin(115200);
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  display.init();                                         // initializing display
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);           //command to rotate orientation of display
  display.clear(); 
}

void draw_live(int count){
  display.clear();
  if (count == 0){
    display.drawString(10, 20, " you death");
  } else if (count == 5){
    display.drawString(20, 20, "full live");
  } else if (count == 1){
    display.fillCircle(64, 32, 10);
  } else if (count == 2){
    display.fillCircle(56, 32, 10);
    display.fillCircle(78, 32, 10);
  } else if (count == 3){
    display.fillCircle(42, 32, 10);
    display.fillCircle(64, 32, 10);
    display.fillCircle(86, 32, 10);
  } else if (count == 4){
    display.fillCircle(32, 32, 10);
    display.fillCircle(54, 32, 10);
    display.fillCircle(76, 32, 10);
    display.fillCircle(98, 32, 10);
  }
  display.display();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /live/1") >= 0){
              Serial.println("GET /live/1");
              draw_live(1);
            } else if (header.indexOf("GET /live/2") >= 0){
              Serial.println("GET /live/2");
              draw_live(2);
            } else if (header.indexOf("GET /live/3") >= 0){
              Serial.println("GET /live/3");
              draw_live(3);
            } else if (header.indexOf("GET /live/4") >= 0){
              Serial.println("GET /live/4");
              draw_live(4);
            } else if (header.indexOf("GET /live/5") >= 0){
              Serial.println("GET /live/5");
              draw_live(5);
            } else if (header.indexOf("GET /live/0") >= 0){
              Serial.println("GET /live/0");              
              draw_live(0);
            } 

            
            
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
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