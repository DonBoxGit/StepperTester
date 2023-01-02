#ifndef _SPEED_TEST_H_
#define _SPEED_TEST_H_
#include <Arduino.h>
#include "fastArduino.h"

inline __attribute__((always_inline))
void test(void) {
    /* Поместить свой код сюда */
    #define BUTTON_PIN 5
    far::pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void startTest(void) {
  Serial.begin(9600);
  TCCR1A = 0x00;              // Обнуление ригистров управления таймер/счетчика Т1
  TCCR1B = 0x00;              
  TCNT1  = 0x00;              // Сброс счетного регистра таймера/счетчика Т1
  TCCR1B |= (1 << CS10) ;     // Режим тактирования тактовый генератор MCU - CLK. Запускаем таймер Т1
  test();                     
  TCCR1B &= ~(1 << CS10);     // Сбрасываем бит CS10. Останавливаем таймер
  uint32_t count = TCNT1 - 2; // минус два такта на действия
  Serial.print("ticks: ");
  Serial.println(count);
  Serial.print("time(us): ");
  Serial.println(count * (float)(1000000.0f / F_CPU), 4);
}

#endif /* _SPEED_TEST_H_ */