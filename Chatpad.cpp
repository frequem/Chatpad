#include "Chatpad.h"

Chatpad::Chatpad(){}

void Chatpad::Init(Stream& serialstream){
  m_serialstream = &serialstream;
  
  m_timer.init(1000);
  
  chatpad_init();
  chatpad_keep_awake();
}

void Chatpad::Init(Stream& serialstream, void (*f_press)(char), void (*f_release)(char)){
  pressHandler(f_press);
  releaseHandler(f_release);

  Init(serialstream);
}

void Chatpad::pressHandler(void (*f_press)(char)){
  handler_onPress = f_press;
}

void Chatpad::releaseHandler(void (*f_release)(char)){
  handler_onRelease = f_release;
}

void Chatpad::bufferUpdateHandler(void (*f_buffer_update)(uint8_t[8])){
  handler_onBufferUpdate = f_buffer_update;
}

void Chatpad::run(){
  if(m_timer.getTimer()){
    chatpad_keep_awake();
  }
  
  if(m_serialstream->available() >= 8){
    m_serialstream->readBytes(m_buffer, 8);
    if((m_buffer[0] != 0xB4) && (m_buffer[0] != 0xA5)){
		chatpad_fix_stream();
		return;
	}
    if(m_buffer[0] == 0xB4){
		chatpad_onBufferUpdate(m_buffer);
		buffer_evaluate();
	}
  }
}

void Chatpad::buffer_evaluate(){
  char k = chatpad_to_char(m_buffer[4], m_buffer[3]);

  if(k != 0 && m_lastkeys[0] == 0){ // k1 is pressed
    chatpad_onPress(k);
    m_lastkeys[0] = k;
    return;
  }
  if(m_lastkeys[0] != 0 && k == 0){ // k1 is released
    chatpad_onRelease(m_lastkeys[0]);
    m_lastkeys[0] = 0;
    return;
  }
  if(k != 0 && k == m_lastkeys[1]){ //k2 switched to k1
    chatpad_onRelease(m_lastkeys[0]);
    m_lastkeys[0] = m_lastkeys[1];
    m_lastkeys[1] = 0;
    return;
  }
  if(k != 0 && m_lastkeys[0] != 0 && k != m_lastkeys[0]){ //k1 changed char (maybe caps?..)
    chatpad_onRelease(m_lastkeys[0]);
    chatpad_onPress(k);
    m_lastkeys[0] = k;
    return;
  }

  k = chatpad_to_char(m_buffer[5], m_buffer[3]);
  
  if(k != 0 && m_lastkeys[1] == 0){ //k2 is pressed
    chatpad_onPress(k);
    m_lastkeys[1] = k;
    return;
  }
  if(m_lastkeys[1] != 0 && k == 0){ // k2 is released
    chatpad_onRelease(m_lastkeys[1]);
    m_lastkeys[1] = 0;
  }
}

void Chatpad::chatpad_onPress(char key){
  if(handler_onPress) handler_onPress(key);
}

void Chatpad::chatpad_onRelease(char key){
  if(handler_onRelease) handler_onRelease(key);
}

void Chatpad::chatpad_onBufferUpdate(uint8_t buf[8]){
  if(handler_onBufferUpdate) handler_onBufferUpdate(buf);
}

void Chatpad::chatpad_fix_stream(){
	for(int i = 1; i < 8; i++){
		if((m_buffer[i] == 0xB4) || (m_buffer[i] == 0xA5)){
			while(m_serialstream->available() < i);
			m_serialstream->readBytes(m_buffer, i);
			break;
		}
	}
}

void Chatpad::chatpad_init(){
  m_serialstream->write(0x87);
  m_serialstream->write(0x02);
  m_serialstream->write(0x8C);
  m_serialstream->write(0x1F);
  m_serialstream->write(0xCC);
  m_serialstream->flush();
}

void Chatpad::chatpad_keep_awake(){
  m_serialstream->write(0x87);
  m_serialstream->write(0x02);
  m_serialstream->write(0x8C);
  m_serialstream->write(0x1B);
  m_serialstream->write(0xD0);
  m_serialstream->flush();
}

char Chatpad::chatpad_to_char(uint8_t chatpad_byte, uint8_t modifier){
  uint8_t c = 0;
  switch(chatpad_byte){
    case 0x65: return '0';
    case 0x17: return '1';
    case 0x16: return '2';
    case 0x15: return '3';
    case 0x14: return '4';
    case 0x13: return '5';
    case 0x12: return '6';
    case 0x11: return '7';
    case 0x67: return '8';
    case 0x66: return '9';
    case 0x37: c = 'a'; break; 
    case 0x42: c = 'b'; break;
    case 0x44: c = 'c'; break;
    case 0x35: c = 'd'; break;
    case 0x25: c = 'e'; break;
    case 0x34: c = 'f'; break;
    case 0x33: c = 'g'; break;
    case 0x32: c = 'h'; break;
    case 0x76: c = 'i'; break;
    case 0x31: c = 'j'; break;
    case 0x77: c = 'k'; break;
    case 0x72: c = 'l'; break;
    case 0x52: c = 'm'; break;
    case 0x41: c = 'n'; break;
    case 0x75: c = 'o'; break;
    case 0x64: c = 'p'; break;
    case 0x27: c = 'q'; break;
    case 0x24: c = 'r'; break;
    case 0x36: c = 's'; break;
    case 0x23: c = 't'; break;
    case 0x21: c = 'u'; break;
    case 0x43: c = 'v'; break;
    case 0x26: c = 'w'; break;
    case 0x45: c = 'x'; break;
    case 0x22: c = 'y'; break;
    case 0x46: c = 'z'; break;
    case 0x54: return ' ';
    case 0x62: c = ','; break;
    case 0x53: c = '.'; break;
    case 0x71: return KEY_BACKSPACE;
    case 0x51: c = KEY_RIGHT_ARROW; break;
    case 0x55: c = KEY_LEFT_ARROW; break;
    default: return 0;
  }
  
  if(modifier & MODIFIER_SHIFT){
    if(c >= 'a' && c <= 'z') return c-32;
    switch(c){
      case KEY_RIGHT_ARROW: return KEY_DOWN_ARROW;
      case KEY_LEFT_ARROW: return KEY_UP_ARROW;
    }
  }
  
  if(modifier & MODIFIER_LEFTCTRL){
	  switch(c){
		  case 'q': return '!';
		  case 'w': return '@';
		  case 'r': return '#';
		  case 't': return '%';
		  case 'y': return '^';
		  case 'u': return '&';
		  case 'i': return '*';
		  case 'o': return '(';
		  case 'p': return ')';
		  case 'a': return '~';
		  case 'd': return '{';
		  case 'f': return '}';
		  case 'h': return '/';
		  case 'j': return '\'';
		  case 'k': return '[';
		  case 'l': return ']';
		  case ',': return ':';
		  case 'z': return '`';
		  case 'v': return '-';
		  case 'b': return '|';
		  case 'n': return '<';
		  case 'm': return '>';
		  case '.': return '?';
	  }
  }

  if(modifier & MODIFIER_RIGHTCTRL){
	  switch(c){
		  case 'r': return '$';
		  case 'p': return '=';
		  case 'h': return '\\';
		  case 'j': return '"';
		  case ',': return ';';
		  case 'v': return '_';
		  case 'b': return '+';
	  }
  }
  
  return c;
}

