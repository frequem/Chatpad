#ifndef CHATPAD_H
#define CHATPAD_H

#include "Arduino.h"
#include <SeqTimer.h>

class Chatpad{
  public:
    Chatpad();
    void Init(Stream& serialstream);
    void Init(Stream& serialstream, void (*f_press)(char), void (*f_release)(char));
    
    void run();

    void pressHandler(void (*f_press)(char));
    void releaseHandler(void (*f_release)(char));

    //debug
    void bufferUpdateHandler(void (*f_buffer_update)(uint8_t[8]));
  private:
    Stream *m_serialstream;
    SeqTimer m_timer;
    uint8_t m_buffer[8];
    char m_lastkeys[2];

    void (*handler_onPress)(char);
    void (*handler_onRelease)(char);
    //debug
    void (*handler_onBufferUpdate)(uint8_t[8]);

    void buffer_evaluate();
    
    void chatpad_onPress(char key);
    void chatpad_onRelease(char key);
    void chatpad_onBufferUpdate(uint8_t buf[8]);
    
    void chatpad_init(); 
    void chatpad_keep_awake();

    static char chatpad_to_char(uint8_t chatpad_byte, uint8_t modifier);
};

#endif
