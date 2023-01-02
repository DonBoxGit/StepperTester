#include "fastArduino.h"

/*----------------------------pinMode function---------------------------------*/
void far::pinMode(uint8_t pin, uint8_t mode) {
  switch (mode) {
    case INPUT:
      if (pin < 8) {
        DDRD  &= ~(1 << pin);   // Mode: Input
        PORTD &= ~(1 << pin);   // State: Hi-Z
      } else if (pin < 14) {
        DDRB  &= ~(1 << (pin - 8));
        PORTB &= ~(1 << (pin - 8));
      } else if (pin < 20) {
        DDRC  &= ~(1 << (pin - 14));
        PORTC &= ~(1 << (pin - 14));
      }
      return;

    case OUTPUT:
      if (pin < 8) {
        DDRD  |=  (1 << pin);   // Mode: Output
        PORTD &= ~(1 << pin);   // State: Low
      } else if (pin < 14) {
        DDRB  |=  (1 << (pin - 8));
        PORTB &= ~(1 << (pin - 8));
      } else if (pin < 20) {
        DDRC  |= (1 <<  (pin - 14));
        PORTC &= ~(1 << (pin - 14));
      }
      return;
      
    case INPUT_PULLUP:
      if (pin < 8) {
        DDRD  &= ~(1 << pin);   // Mode: Input
        PORTD |=  (1 << pin);   // State: Pull-Up
      } else if (pin < 14) {
        DDRB  &= ~(1 << (pin - 8));
        PORTB |=  (1 << (pin - 8));
      } else if (pin < 20) {
        DDRC  &= ~(1 << (pin - 14));
        PORTC |=  (1 << (pin - 14));
      }
      return;
  }
}

/*-----------------------------digitalRead function----------------------------*/
bool far::digitalRead(uint8_t pin) {
    bool state;
    if (pin < 8)       state = PIND & (1 << pin);
    else if (pin < 14) state = PINB & (1 << (pin - 8));
    else if (pin < 20) state = PINC & (1 << (pin - 14));
    return state;
}

/*---------------------------digitalWrite function-----------------------------*/
void far::digitalWrite(uint8_t pin, bool state) {
    switch (pin) {   /* PWM disable */
      case 3:  TCCR2A &= ~(1 << COM2B1); break;
      case 5:  TCCR0A &= ~(1 << COM0B1); break;
      case 6:  TCCR0A &= ~(1 << COM0A1); break;
      case 9:  TCCR1A &= ~(1 << COM1A1); break;
      case 10: TCCR1A &= ~(1 << COM1B1); break;
      case 11: TCCR2A &= ~(1 << COM2A1); break;
    }
    /* Set pin to HIGH or LOW state */
    if (pin < 8)       state ? PORTD |= (1 << pin)        : PORTD &= ~(1 << pin);
    else if (pin < 14) state ? PORTB |= (1 << (pin - 8))  : PORTB &= ~(1 << (pin - 8));
    else if (pin < 20) state ? PORTC |= (1 << (pin - 14)) : PORTC &= ~(1 << (pin - 14));
}

/*--------------------------digitalToggle function---------------------------*/
void far::digitaToggle(uint8_t pin) {
    if (pin < 8)       PIND ^= (1 << pin);
    else if (pin < 14) PINB ^= (1 << (pin - 8));
    else if (pin < 20) PINC ^= (1 << (pin - 14));
}

/*-----------------------Uart class instead of Serial------------------------*/
void Uart::begin(uint32_t baudrate) {
      uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
      UBRR0 = speed;
      UCSR0A = (1 << U2X0);
      UCSR0B = (1 << TXEN0) | (1 << RXEN0);
      UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

void Uart::end() {
      UCSR0B = 0;
    }

void Uart::sendByte(uint8_t data) {
      while (!(UCSR0A & (1 << UDRE0)));
      UDR0 = data;
    }

void Uart::sendArray(uint8_t *buffer, uint16_t bufferSize) {
      for(uint16_t i = 0; i < bufferSize - 1; i++)
        sendByte(buffer[i]);
    }

bool Uart::available() {
      return (UCSR0A & (1 << RXC0));
    }

char Uart::readByte() {
      uint8_t data = UDR0;
      return data;
    }
