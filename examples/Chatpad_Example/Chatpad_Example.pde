/*
|| @author         frequem
|| @url            https://github.com/frequem/Chatpad/
||
|| @description
|| |
|| | This is an example sketch using the Chatpad Wiring Library.
|| |
|| #
||
|| @license Please see the accompanying LICENSE.txt file for this project.
||
*/

#include "Chatpad.h"

Chatpad chatpad;// Create a Chatpad object.
AltSoftSerial chatpadSerial;// Serial connection to the chatpad

void setup(){
  Serial.begin(9600); //USB Serial for printing to Serial Monitor
  chatpadSerial.begin(19200); // always 19200 for the chatpadSerial, cannot be moved into chatpad class since that takes a stream object and all streams shall be supported
  chatpad.Init(chatpadSerial, onPress, onRelease);
}

void loop(){
  chatpad.run(); //important
}

void onPress(char key){
  Serial.print("pressing ");
  Serial.println(key);
}

void onRelease(char key){
  Serial.print("releasing ");
  Serial.println(key);
}

