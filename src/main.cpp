#include <Arduino.h>

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "TPLink_repeat";
const char* password = "VcnS4ybmz";

// Set web server port number to 80
WiFiServer server(80);

// Assign output variables to GPIO pins
const int output21 = 21;
const int output18_g = 18;
const int output17_r = 17;
const int output16_b = 16;

bool blue_state = false;
bool red_state = false;
bool green_state = false;


void setup() {
  Serial.begin(115200);

  pinMode(output21, OUTPUT);
  pinMode(output18_g, OUTPUT);
  pinMode(output17_r, OUTPUT);
  pinMode(output16_b, OUTPUT);

  digitalWrite(output21, HIGH); 

  digitalWrite(output18_g, HIGH);
  digitalWrite(output17_r, HIGH);
  digitalWrite(output16_b, HIGH);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.begin();


}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if(client){
    Serial.println("new client");   

    String currentLine = "";
    String CL_no_get = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();            
        Serial.write(c);

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/HLb\">here</a> turn the blue LED on<br>"); // BLUE
            
            client.print("Click <a href=\"/HLr\">here</a> turn the red LED on<br>"); // RED

            client.print("Click <a href=\"/HLg\">here</a> turn the red LED on<br>"); // GREEN

            client.println();

            CL_no_get = currentLine;
            break;
          }else{
            currentLine = "";
          }
        }else if (c != '\r')
        {
          currentLine += c;
        }
        /////////////////////////////////// BLUE /////////////////////////////////////////////
        if (currentLine.endsWith("GET /HLb") & blue_state == false) {
          digitalWrite(output16_b, LOW);                 // GET /Hb turns the blue LED on
          blue_state = true;
          currentLine = CL_no_get; // clean the line
        }
        if (currentLine.endsWith("GET /HLb") & blue_state == true) {
          digitalWrite(output16_b, HIGH);                // GET /Hb turns the blue LED off
          blue_state = false;
          currentLine = CL_no_get; // clean the line
        }

        ////////////////////////////////// RED ///////////////////////////////////////////////
        if (currentLine.endsWith("GET /HLr") & red_state == false) {
          digitalWrite(output17_r, LOW);                 // GET /Hr turns the red LED on
          red_state = true;
          currentLine = CL_no_get; // clean the line
        }
        if (currentLine.endsWith("GET /HLr") & red_state == true) {
          digitalWrite(output17_r, HIGH);                // GET /Hr turns the red LED off
          red_state = false;
          currentLine = CL_no_get; // clean the line
        }        

        ////////////////////////////////// GREEN /////////////////////////////////////////////
        if (currentLine.endsWith("GET /HLg") & green_state == false) {
          digitalWrite(output18_g, LOW);                 // GET /Hr turns the green LED on
          green_state = true;
          currentLine = CL_no_get; // clean the line
        }
        if (currentLine.endsWith("GET /HLg") & green_state == true) {
          digitalWrite(output18_g, HIGH);                // GET /Hr turns the green LED off
          green_state = false;
          currentLine = CL_no_get; // clean the line
        }    
        
      }
    }
    client.stop();
    Serial.println("client disonnected");
  }
}

