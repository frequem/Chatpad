#include <Chatpad.h>
#include <Keyboard.h>

Chatpad chatpad;

void setup() {
  Keyboard.begin();
  Serial.begin(9600);
  Serial1.begin(19200);
  chatpad.Init(Serial1, onPress, onRelease);
}

void loop(){
  chatpad.run();
}

void onPress(char key){
  Keyboard.press(key);
}

void onRelease(char key){
  Keyboard.release(key);
}

