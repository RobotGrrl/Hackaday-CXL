// LoRa Gateway:::

// include libraries
#include <WiFi.h>
#include <WebServer.h>

#include <SPI.h>
#include <LoRa.h>


// Replace with your network credentials
const char* ssid = "EJA_LoRa";
const char* password = "12345678";



IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);


bool LED1status = LOW;



// --> 433 MHz, 868 MHz (Europe), 
//     915 MHz (Australia and North America) and 
//     923 MHz (Asia).
// LoRa Frequency
const long frequency = 915E6;

// LoRa radio chip select
const int csPin = 2;
// LoRa radio reset
const int resetPin = 15;
// depends on board; must be a hardware interrupt pin
const int irqPin = 13;

void setup()
{
  // initialize serial
  Serial.begin(115200);
  while (!Serial);

  // LoRa pin setup
  LoRa.setPins(csPin, resetPin, irqPin);

  // wait up to successfully LoRa initialization
  if (!LoRa.begin(frequency))
  {
    Serial.println("LoRa init failed. Check your connections.");
    // if failed, do nothing
//    while (true);
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from Nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();

  // onReceive callback
  LoRa.onReceive(Gateway_onReceive_callback);
  // onTxDone callback
  LoRa.onTxDone(Gateway_onTxDone_callback);
  IoT_GatewayLoRa_into_rxMode();

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);

  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");

}

void loop() 
{
  // repeat every 5 sec interval
  if (runEvery(5000)) 
  {
    String message = "Message from Gateway: ";
    message += "Gateway working!!!!! ";
    message += millis();
    // send a message to LoRa Node
    LoRa_sendMessagetoNode(message);
    Serial.println("Send Message to Node!");
  }
  server.handleClient();
}

// function for Gateway LoRa into RX mode
void IoT_GatewayLoRa_into_rxMode()
{
  // normal mode
  LoRa.disableInvertIQ();
  // set receive mode
  LoRa.receive();
}

// function for Gateway LoRa into TX mode
void IoTLoRa_into_txMode()
{
  // set standby mode
  LoRa.idle();
  // active invert I and Q signals
  LoRa.enableInvertIQ();
}

// function to send message to LoRa Node
void LoRa_sendMessagetoNode(String message) 
{
  // set tx mode
  IoTLoRa_into_txMode();
  // start packet
  LoRa.beginPacket();
  // add payload
  LoRa.print(message);
  // finish packet and send it
  LoRa.endPacket(true);
}

// callback function for receive packet from LoRa Node
void Gateway_onReceive_callback(int packetSize) 
{
  // to store message from LoRa Node
  String message = "";
  // Node LoRa any message that parse into "message"
  while (LoRa.available()) 
  {
    message += (char)LoRa.read();
  }
  Serial.print("IoT Gateway Receive: ");
  Serial.println(message);
}

// callback function for successfully receive packet from LoRa Node
void Gateway_onTxDone_callback() 
{
  Serial.println("IoT_GatewayTxDone");
  // LoRa setup into RX mode
  IoT_GatewayLoRa_into_rxMode();
}

// function to run every interval setup
boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

void handle_led1on() {
    String message = "ON_REQUEST";
    // send a message to LoRa Node
    LoRa_sendMessagetoNode(message);
    Serial.println("On --> Send Message to Node!");
  LED1status = HIGH;
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_led1off() {
    String message = "OFF_REQUEST";
    // send a message to LoRa Node
    LoRa_sendMessagetoNode(message);
    Serial.println("Off --> Send Message to Node!");
  LED1status = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>EJA_LoRa Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
