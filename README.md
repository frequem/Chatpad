Chatpad
==============

## Description

A [Arduino](https://en.wikipedia.org/wiki/Arduino)/[Wiring](https://en.wikipedia.org/wiki/Wiring_(development_platform)) library for connectiong to Microsoft's XBOX chatpad.

This library does NOT work in conjunction with SoftwareSerial, since that does not support simultaneous sending and receiving.

It does however work with [AltSoftSerial by PaulStoffregen](https://github.com/PaulStoffregen/AltSoftSerial).

[SMFSW's SeqTimer](https://github.com/SMFSW/SeqTimer) is needed to compile this library.

## Example

```cpp
#include "Chatpad.h"

Chatpad chatpad;// Create a Chatpad object.
AltSoftSerial chatpadSerial;// Serial connection to the chatpad

void setup(){
  Serial.begin(9600); //USB Serial for printing to Serial Monitor
  
  //can be replaced with one of the hardware serials
  //cannot be moved into chatpad class since that takes a stream object and all streams shall be supported
  chatpadSerial.begin(19200); // always 19200 for the chatpadSerial
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

```


