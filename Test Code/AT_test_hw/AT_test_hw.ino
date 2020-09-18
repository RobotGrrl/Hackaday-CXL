// https://tlfong01.blog/2020/03/22/sim7600-at-command-tutorial/
// https://wiki.dfrobot.com/SIM7600CE-T_4G(LTE)_Shield_V1.0_SKU_TEL0124
// https://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/at-cmgs-text-mode/
// https://github.com/HologramEducation/hologram-SIMCOM

long current_time = 0;
long last_at = 0;

void setup()
{
  Serial2.begin(115200); //Initialize virtual serial port
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
        Serial2.println("AT");
        Serial.println("sent a");
      break;
      case 'b': // prepare to send sms
        Serial2.println("AT+CMGS=\"+16136509466\";");
        Serial.println("sent b");
      break;
      case 'B':
        Serial2.println("Testing 123456 Eja 1234");
        Serial2.write(0x1A);
        Serial.println("sent B");
      break;
      case 'C':
        Serial2.println((int)random(0,256));
        Serial2.write(0x1A);
        Serial.println("sent C");
      break;
      case 'c': // test call
        Serial2.println("ATD16136509466;");
        Serial.println("sent c");
      break;
      case 'd': // model number
        Serial2.println("AT+CGMM;");
        Serial.println("sent d");
      break;
      case 'e': // hardware revision
        Serial2.println("AT+CGMR;");
        Serial.println("sent e");
      break;
      case 'f': // module capability
        Serial2.println("AT+GCAP;");
        Serial.println("sent f");
      break;
      case 'g': // read the profiles stored
        Serial2.println("AT&V;");
        Serial.println("sent g");
      break;
      case 'h': // get the port speed
        Serial2.println("AT+IPR?");
        Serial.println("sent h");
      break;
      case 'i': // get the module activity status
        Serial2.println("AT+CPAS;");
        Serial.println("sent i");
      break;
      case 'j': // get the clock
        Serial2.println("AT+CCLK?");
        Serial.println("sent j");
      break;
      case 'k': // get the gprs station class
        Serial2.println("AT+CGCLASS?");
        Serial.println("sent k");
      break;
      case 'l': // check if connected to the network
        Serial2.println("AT+CGACT?");
        Serial.println("sent l");
      break;
      case 'm': // check signal strength
        // the signal strength range is -53 dbm (Excellent) to -109 dbm (Marginal).
        Serial2.println("AT+CSQ;");
        Serial.println("sent m");
      break;
      case 'n': // check current operator
        Serial2.println("AT+COPS?");
        Serial.println("sent n");
      break;
      case 'o': // check registration status
        Serial2.println("AT+CREG?");
        Serial.println("sent o");
      break;
      case 'p': // check band setting
        Serial2.println("AT+CBAND?");
        Serial.println("sent p");
      break;
      case 'q': // check if device is ready for call
        Serial2.println("AT+CCALR?");
        Serial.println("sent q");
      break;
      case 'r': // * check if sim is ready
        Serial2.println("AT+CPIN?");
        Serial.println("sent r");
      break;
      case 's': // * check cell strength
        Serial2.println("AT+CSQ;");
        Serial.println("sent s");
      break;
      case 't': // * set functionality to text
        Serial2.println("AT+CMGF=1;");
        Serial.println("sent t");
      break;
      // connecting to network - skipping shutting down modem though
      case 'u': // check gprs status
        Serial2.println("AT+CGATT?");
        Serial.println("sent u");
      break;
      case 'v': // Set connection mode to multi
        Serial2.println("AT+CIPMUX=1;");
        Serial.println("sent v");
      break;
      case 'w': // Set APN
        Serial2.println("AT+CSTT=\"hologram\";");
        Serial.println("sent w");
      break;
      case 'x': // Bring up wireless connection
        Serial2.println("AT+CIICR;");
        Serial.println("sent x");
      break;
      case 'y': // Get local IP address
        Serial2.println("AT+CIFSR;");
        Serial.println("sent y");
      break;
      
      
    }
  }
    
  if(Serial2.available()) {
    char c = Serial2.read();
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
