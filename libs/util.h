#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 8
#define PB1 9
#define PB2 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15

#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define byte uint8_t
#define bit(b) (1UL << (b))

#define INPUT  0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

static const _TO_PIN[] = {1,2,4,8,16,32,64,128};

uint8_t _get_port(uint8_t pinport){
   return pinport >> 8;
}

uint8_t _get_pin(uint8_t portpin){
   return pgm_read_byte(&_TO_PIN + (portpin & 0x07));
}

/*uint8_t digitalRead(uint8_t portpin){
   uint8_t pin = _get_pin(portpin);
   switch(_get_port(portpin)) {
      case 0:
         return (PINA & pin)  >> (portpin & 0x07);
         break;
      case 1:
         return (PINB & pin)  >> (portpin & 0x07); 
         break;
   }

}*/

void digitalWrite(uint8_t portpin, uint8_t val){
   uint8_t pin = _get_pin(portpin);
   switch(_get_port(portpin)) {
      case 0:
         if (val == HIGH) {PORTA |= pin;} else {PORTA &= ~pin;};
         break;
      case 1:
          if (val == HIGH) {PORTB |= pin;} else {PORTB &= ~pin;};
         break;
   }
}

/*void pinMode(uint8_t portpin, uint8_t mode) {
   uint8_t pin = _get_pin(portpin);
   switch(_get_port(portpin)) {
      case 0:
         if (mode == OUTPUT) {DDRA |= pin;} else {DDRA &= ~pin;};
         break;
      case 1:
          if (mode == OUTPUT) {DDRB |= pin;} else {DDRB &= ~pin;};
         break;
   }
}*/



