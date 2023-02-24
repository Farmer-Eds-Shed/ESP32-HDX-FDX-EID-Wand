//https://github.com/T-vK/ESP32-BLE-Keyboard
//https://forum.arduino.cc/t/serial-input-basics/278284

//include BleKeyboard Library
#include "BleKeyboard.h"
BleKeyboard bleKeyboard;

//Variables to store incoming Serial Data
const byte numChars = 17;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

//define serial 2 pin numbers
#define RXD2 16
#define TXD2 17




void setup() {
  //Setup Serial Ports
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  //Setup BleKeyboard
  bleKeyboard.begin(); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

}

void loop() {
  recvWithEndMarker();
  showNewData();
}

//Receive Data until <CR>
void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\r';
 char rc;
 
 // 
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
//country Code  
  if (newData == true) {
    receivedChars[3] = 0;
    Serial.print("EID Tag Read ... ");
    Serial.print(receivedChars);
    bleKeyboard.print(receivedChars);
//Rest of tag
  for (byte i = 4; i < 16; i++) {
    Serial.print(receivedChars[i]);
    bleKeyboard.print(receivedChars[i]);
  }
//Send Return Key  
  Serial.println("");
  bleKeyboard.write(KEY_RETURN);
  delay(1000);

 newData = false;
 }
}

