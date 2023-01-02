#ifndef _TESTIO_H_
#define _TESTIO_H_

#include <Arduino.h>
#include "config.h"
#include "fastArduino.h"

void test(void) {
    far::digitalRead(TERM_SW_PIN_1);
    far::digitalRead(TERM_SW_PIN_2);
}

#endif /* _TESTIO_H_ */