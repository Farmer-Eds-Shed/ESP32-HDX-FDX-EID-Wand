//https://github.com/T-vK/ESP32-BLE-Keyboard
//https://forum.arduino.cc/t/serial-input-basics/278284

//include BleKeyboard Library
#include "BleKeyboard.h"
BleKeyboard bleKeyboard;

//Variables to store incoming RFID Serial Data
const byte numChars = 17;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

//Variables to store incoming Serial Data
char S1receivedChar; // store the received data
boolean S1newData = false;


//define serial 2 pin numbers
#define RXD2 16
#define TXD2 17

void setup() {
  //Setup Serial Port 1 for Debug/Config
  Serial.begin(9600);
  //Serial Port 2 connected to EID Reader
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  //Setup BleKeyboard
  bleKeyboard.begin(); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  recvSerialOptions();
  serialOptions();
  recvWithEndMarker();
  showNewData();
}

//Read serial Data for Config options
void recvSerialOptions() {
 static byte ndx = 0;
 char endMarker = '\r';
 char rc;
 
    while (Serial.available() > 0 && newData == false) {
        S1receivedChar = Serial.read();
        S1newData = true;
  }
}

// Return Config Option selection via serial
void serialOptions() {
  if (S1newData == true) {

    Serial.print(" Option ");
    Serial.print(S1receivedChar);
    Serial.println(" Selected");

 }
 S1newData = false;
}

//Receive EID Data until <CR>
void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\r';
 char rc;
 
    while (Serial2.available() > 0 && newData == false) {
      rc = Serial2.read();

      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
          if (ndx >= numChars) {
          ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

//Send to Serial & BleKeyboard
void showNewData() {
  if (newData == true) {
    Serial.print("EID Tag Read ... ");
    Serial.println(receivedChars);

//full tag
if (S1receivedChar != '4' && S1receivedChar != '5' && S1receivedChar != '3') {
  //country code
  for (byte i = 0; i < 3; i++) {
    bleKeyboard.print(receivedChars[i]);
    delay(5);
  }

//space may not be needed or possibly made optional??
  bleKeyboard.print(" ");
  delay(5);

//Rest of tag
  for (byte i = 4; i < 16; i++) {
    bleKeyboard.print(receivedChars[i]);
    delay(5);
  }
}

//last 3 digits only
if (S1receivedChar == '3') {
  for (byte i = 13; i < 16; i++) {
    bleKeyboard.print(receivedChars[i]);
    delay(5);
  }
}

//last 4 digits only
if (S1receivedChar == '4') {
  for (byte i = 12; i < 16; i++) {
    bleKeyboard.print(receivedChars[i]);
    delay(5);
  }
}

//last 5 digits only
if (S1receivedChar == '5') {
  for (byte i = 11; i < 16; i++) {
    bleKeyboard.print(receivedChars[i]);
    delay(5);
  }
}

//Send Return Key  
    bleKeyboard.write(KEY_RETURN);
    delay(100);
 }
 newData = false;
}

