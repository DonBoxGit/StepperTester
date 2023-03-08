#ifndef _FAST_ARDUINO_H_
#define _FAST_ARDUINO_H_

#include <avr/io.h>

#if !defined Arduino_h
  enum Mode : uint8_t {
        INPUT = 0,
        OUTPUT,
        INPUT_PULLUP,
    };
#endif /* !defined Arduino_h */

namespace far {
void pinMode(uint8_t pin, uint8_t mode);
bool digitalRead(uint8_t pin);
void digitalWrite(uint8_t pin, bool state);
void digitaToggle(uint8_t pin);
} /* far namespace */

/*---------------------| Uart class instead of Serial |----------------------*/
class Uart /*: public Print */ {
  public:
    void begin(uint32_t baudrate);
    void end(void);
    void sendByte(uint8_t data);
    void sendArray(uint8_t *buffer, uint16_t bufferSize);
    bool available(void);
    char readByte(void); 
};

#endif /* _FAST_ARDUINO_H_ */