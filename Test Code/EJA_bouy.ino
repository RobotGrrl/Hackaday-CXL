// LoRA Node:::

// include libraries

#include <SPI.h>
#include <LoRa.h>

// Set LED GPIO
const int ledPin = 23;

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
    while (true);
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from Gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

  // onReceive callback
  LoRa.onReceive(Node_onReceive_callback);
  // onTxDone callback
  LoRa.onTxDone(Node_onTxDone_callback);
  IoT_NodeLoRa_into_rxMode();
}

void loop() {
  // repeat every 5 sec interval
  if (runEvery(5000)) 
  {
    String message = "Message from Node: ";
    message += "Node working!!!!! ";
    message += millis();
    // send a message to LoRa Gateway
    LoRa_sendMessagetoGateway(message);
    Serial.println("Send Message to Gateway!");
  }
}

// function for Node LoRa into RX mode
void IoT_NodeLoRa_into_rxMode()
{
  // active invert I and Q signals
  LoRa.enableInvertIQ();
  // set receive mode
  LoRa.receive();
}

// function for Node LoRa into TX mode
void IoTLoRa_into_txMode()
{
  // set standby mode
  LoRa.idle();
  // normal mode
  LoRa.disableInvertIQ();
}

// function to send message to LoRa Gateway
void LoRa_sendMessagetoGateway(String message) 
{
  // set tx mode
  IoTLoRa_into_txMode();
  // start packet
  LoRa.beginPacket();
  // add payload
  LoRa.print(message);
  // finish packet and send it to Gateway
  LoRa.endPacket(true);
}

// callback function for receive packet from LoRa Gateway
void Node_onReceive_callback(int packetSize) 
{
  // to store message from LoRa Gateway
  String message = "";
  // Node LoRa any message that parse into "message"
  while (LoRa.available()) 
  {
    message += (char)LoRa.read();
  }
  Serial.print("IoT Node Receive: ");
  Serial.println(message);
  if(message == "ON_REQUEST")
  {
    digitalWrite(ledPin,HIGH);
    Serial.println("receive ON request from gateway LoRa");
  }
  else if(message == "OFF_REQUEST")
  {
    digitalWrite(ledPin,LOW);
    Serial.println("receive OFF request from gateway LoRa");
  }
}

// callback function for successfully receive packet from LoRa Gateway
void Node_onTxDone_callback() 
{
  Serial.println("IoT_NodeTxDone");
  // Node LoRa setup into RX mode
  IoT_NodeLoRa_into_rxMode();
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
