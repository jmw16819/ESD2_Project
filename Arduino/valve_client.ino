/*
https://github.com/jmw16819/ESD2_Project
WeFlow - a water leak detection system
by Jordyn Washington 2021 - MIT License
*/

/* Code to create a WebSocket connection between the solenoid valves and the monitor webpage,
 * as well as change their states as needed. Utilizes the ArduinoWebsockets library by
 * gilmaimon on Github. Server will use index.html, monitor.py, monitor.js, and 
 * sensor_client.ino.
 * Note: Make sure ArduinoWebsockets is always up to date.
*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>

// Save the ESP32 dev board MAC addresses
const char* v1_mac = "";
const char* v2_mac = "";
const char* v3_mac = "";

const char* ssid = "ssid";
const char* password = "password";
const char* websocket_server = "";

using namespace websockets;
WebsocketsClient client;

// Function to set the right dev board to the correct websocket tag
void server_setup(void){
  if (WiFi.macAddress()==v1_mac){
    websocket_server = "ws://" + localhost + ":8888/v1socket";
    delay(500);
    bool connected = client.connect(websocket_server);
    if (connected){
      // Send a message
      client.send("Ready to send data");
    }
    else{
      Serial.println("Not connected.");
    }
  }
  else if (WiFi.macAddress()==v2_mac){
    websocket_server = "ws://" + localhost + ":8888/v2socket";
    delay(1500);
    bool connected = client.connect(websocket_server);
    if (connected){
      // Send a message
      client.send("Ready to send data");
    }
    else{
      Serial.println("Not connected.");
    }
  }
  else if (WiFi.macAddress()==v3_mac){
    websocket_server = "ws://" + localhost + ":8888/v3socket";
    delay(2500);
    bool connected = client.connect(websocket_server);
    if (connected){
      // Send a message
      client.send("Ready to send data");
    }
    else{
      Serial.println("Not connected.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);
  for(int i=0; i<10 && WiFi.status() != WL_CONNECTED; i++){
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi not connected.");
  }
  
  pinMode(18, OUTPUT); // sets the input to the relay
  pinMode(25, OUTPUT); // sets the level of the red indicator LED
  pinMode(26, OUTPUT); // sets the level of the green indicator LED
  digitalWrite(18, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, HIGH);
  Serial.println("Start");

  // Set up callback functions
  client.onMessage([&](WebsocketsMessage message){
    if (WiFi.macAddress()==v1_mac){
      if (message.data()=="Valve 1 change: Open valve 1"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, LOW);
        digitalWrite(25, LOW);
        digitalWrite(26, HIGH);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 1 is closed.");
        }
        else{
          Serial.println("Valve 1 is open.");
        }
        client.send("Valve 1 open");
      }
      else if (message.data()=="Valve 1 change: Close valve 1"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, HIGH);
        digitalWrite(25, HIGH);
        digitalWrite(26, LOW);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 1 is closed.");
        }
        else{
          Serial.println("Valve 1 is open.");
        }
        client.send("Valve 1 closed");
      }
    }
    else if (WiFi.macAddress()==v2_mac){
      if (message.data()=="Valve 2 change: Open valve 2"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, LOW);
        digitalWrite(25, LOW);
        digitalWrite(26, HIGH);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 2 is closed.");
        }
        else{
          Serial.println("Valve 2 is open.");
        }
        client.send("Valve 2 open");
      }
      else if (message.data()=="Valve 2 change: Close valve 2"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, HIGH);
        digitalWrite(25, HIGH);
        digitalWrite(26, LOW);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 2 is closed.");
        }
        else{
          Serial.println("Valve 2 is open.");
        }
        client.send("Valve 2 closed");
      }
    }
    else if (WiFi.macAddress()==v3_mac){
      if (message.data()=="Valve 3 change: Open valve 3"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, LOW);
        digitalWrite(25, LOW);
        digitalWrite(26, HIGH);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 3 is closed.");
        }
        else{
          Serial.println("Valve 3 is open.");
        }
        client.send("Valve 3 open");
      }
      else if (message.data()=="Valve 3 change: Close valve 3"){
        Serial.println("Message received: " + message.data());
        digitalWrite(18, HIGH);
        digitalWrite(25, HIGH);
        digitalWrite(26, LOW);
        if (digitalRead(18)==HIGH){
          Serial.println("Valve 3 is closed.");
        }
        else{
          Serial.println("Valve 3 is open.");
        }
        client.send("Valve 3 closed");
      }
    }
  });
  client.onEvent([&](WebsocketsEvent event, String data){
    if(event == WebsocketsEvent::ConnectionOpened){
      Serial.println("Connection opened.");
    }
    else if(event == WebsocketsEvent::ConnectionClosed){
      Serial.println("Connection closed.");
      // Try to reconnect to server
      bool connected = client.connect(websocket_server);
      if (connected){
        // Send a message
        client.send("Ready to send data");
      }
      else{
        Serial.println("Not connected.");
      }
    }
  });
  
  // Connect to server
  server_setup();
}

void loop() {
  if (client.available()){
    client.poll(); // included so the client can keep receiving messages    
  }
}
