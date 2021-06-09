/*
https://github.com/jmw16819/ESD2_Project
WeFlow - a water leak detection system
by Jordyn Washington 2021 - MIT License
*/

/* Code to create a WebSocket connection between the pressure sensors and the monitor 
 * webpage. Utilizes the ArduinoWebsockets library by gilmaimon on Github. Server will use 
 * index.html, monitor.py, monitor.js, and valve_client.ino.
 * Note: Make sure ArduinoWebsockets is always up to date.
*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>

// Save the ESP32 dev board MAC addresses
const char* s1_mac = "";
const char* s2_mac = "";
const char* s3_mac = "";

const char* ssid = "ssid";
const char* password = "password";
const char* websocket_server = "";

using namespace websockets;
WebsocketsClient client;

// Function to set the right dev board to the correct websocket tag
void server_setup(void){
  if (WiFi.macAddress()==s1_mac){
    websocket_server = "ws://" + localhost + ":8888/s1socket";
    delay(0);
    bool connected = client.connect(websocket_server);
    if (connected){
      // Send a message
      client.send("Ready to send data");
    }
    else{
      Serial.println("Not connected.");
    }
  }
  else if (WiFi.macAddress()==s2_mac){
    websocket_server = "ws://" + localhost + ":8888/s2socket";
    delay(1000);
    bool connected = client.connect(websocket_server);
    if (connected){
      // Send a message
      client.send("Ready to send data");
    }
    else{
      Serial.println("Not connected.");
    }
  }
  else if (WiFi.macAddress()==s3_mac){
    websocket_server = "ws://" + localhost + ":8888/s3socket";
    delay(2000);
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

// Sensor output
volatile int sensor_out = 0;
// Sensor output converted to PSI
volatile float pressure_read = 0;
char num[8];
char mess[100];

// Functions to adjust LEDs
void high_update(void){
  digitalWrite(25, LOW);
  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);
}
void mid_update(void){
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, HIGH);
}
void low_update(void){
  digitalWrite(25, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
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
  
  pinMode(25, OUTPUT); // sets the level of the red indicator LED
  pinMode(26, OUTPUT); // sets the level of the green indicator LED
  pinMode(27, OUTPUT); // sets the level of the yellow indicator LED
  digitalWrite(25, LOW);
  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);
  Serial.println("Start");

  // Set up callback functions
  client.onMessage([&](WebsocketsMessage message){
    if (WiFi.macAddress()==s1_mac){
      if (message.data() == "Sensor 1: Data received" || message.data() == "Ready to receive data."){
        Serial.println("Message received: " + message.data());
        strcpy(mess, "Sensor 1 pressure: ");
        strcat(mess, dtostrf(pressure_read, 6, 2, num));
        strcat(mess, "; ");
        if (digitalRead(25)==HIGH){
          strcat(mess, "LED: red 1, ");
        }
        else{
          strcat(mess, "LED: red 0, ");
        }
        if (digitalRead(27)==HIGH){
          strcat(mess, "yellow 1, ");
        }
        else{
          strcat(mess, "yellow 0, ");
        }
        if (digitalRead(26)==HIGH){
          strcat(mess, "green 1");
        }
        else{
          strcat(mess, "green 0");
        }
        Serial.println(mess);
        client.send(mess);
      }
    }
    else if (WiFi.macAddress()==s2_mac){
      if (message.data() == "Sensor 2: Data received" || message.data() == "Ready to receive data."){
        Serial.println("Message received: " + message.data());
        strcpy(mess, "Sensor 2 pressure:");
        strcat(mess, dtostrf(pressure_read, 6, 2, num));
        strcat(mess, "; ");
        if (digitalRead(25)==HIGH){
          strcat(mess, "LED: red 1, ");
        }
        else{
          strcat(mess, "LED: red 0, ");
        }
        if (digitalRead(27)==HIGH){
          strcat(mess, "yellow 1, ");
        }
        else{
          strcat(mess, "yellow 0, ");
        }
        if (digitalRead(26)==HIGH){
          strcat(mess, "green 1");
        }
        else{
          strcat(mess, "green 0");
        }
        Serial.println(mess);
        client.send(mess);
      }
    }
    else if (WiFi.macAddress()==s3_mac){
      if (message.data() == "Sensor 3: Data received" || message.data() == "Ready to receive data."){
        Serial.println("Message received: " + message.data());
        strcpy(mess, "Sensor 3 pressure:");
        strcat(mess, dtostrf(pressure_read, 6, 2, num));
        strcat(mess, "; ");
        if (digitalRead(25)==HIGH){
          strcat(mess, "LED: red 1, ");
        }
        else{
          strcat(mess, "LED: red 0, ");
        }
        if (digitalRead(27)==HIGH){
          strcat(mess, "yellow 1, ");
        }
        else{
          strcat(mess, "yellow 0, ");
        }
        if (digitalRead(26)==HIGH){
          strcat(mess, "green 1");
        }
        else{
          strcat(mess, "green 0");
        }
        Serial.println(mess);
        client.send(mess);
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
  sensor_out = analogRead(A0);
  // Conversion of analog input to PSI
  pressure_read = sensor_out / 33.62;
  // Adjust the pressure bounds if needed
  if (pressure_read >= 10.0){
    high_update();
  }
  else if (pressure_read < 10.0 && pressure_read >= 5.0){
    mid_update();
  }
  else if (pressure_read < 5.0) {
    low_update();
  }
  delay(500);
  
  if (client.available()){
    client.poll(); // included so the client can keep receiving messages    
  }
}
