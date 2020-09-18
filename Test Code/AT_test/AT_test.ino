#include <SoftwareSerial.h>
SoftwareSerial myserial(7, 8); //Define virtual serial port name as myseria,Rx is port 7, Tx is port 8

long current_time = 0;
long last_at = 0;

void setup()
{
  myserial.begin(115200); //Initialize virtual serial port
  Serial.begin(115200); //Initialize Arduino default serial port
}

void loop()
{

  current_time = millis();

  /*
  if(current_time-last_at >= 2000) {
    myserial.println("AT");
    Serial.println("sent AT");
    last_at = current_time;
  }
  */


  if(Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'a':
        myserial.println("AT");
        Serial.println("sent a");
      break;
      case 'b':
        myserial.println("ATD170****9217;");
        Serial.println("sent b");
      break;
      case 'c':
        myserial.println("ATD17091319217;");
        Serial.println("sent c");
      break;
      case 'd': // model number
        myserial.println("AT+CGMM;");
        Serial.println("sent d");
      break;
      case 'e': // hardware revision
        myserial.println("AT+CGMR;");
        Serial.println("sent e");
      break;
      case 'f': // module capability
        myserial.println("AT+GCAP;");
        Serial.println("sent f");
      break;
    }
  }
    
  if(myserial.available()) {
    char c = myserial.read();
    Serial.print(c);
    //Serial.println(myserial.read());//if Serial received data, output it via mySerial.
  }
  /*
  if(Serial.available()) {
    char c = Serial.read();
    myserial.print(c);
    //myserial.write(Serial.read());//if myserial received data, output it via Serial.
    Serial.print("written");
  }
  */
}
