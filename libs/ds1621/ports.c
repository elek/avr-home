// Ports library definitions
// 2009-02-13 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include "ports.h"
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define OUTPUT 0x1
#define INPUT 0x0

#define SDA_RD		DDRD
#define SDA_PORT	PORTD
#define SDA_PIN		6

#define SCL_RD		DDRC
#define SCL_PORT	PORTC
#define SCL_PIN		2

void hold() { _delay_us(34);}

void sdaOut(uint8_t value) 
        { bitWrite(SDA_RD, SDA_PIN, 1); bitWrite(SDA_PORT, SDA_PIN, value); }

inline uint8_t sdaIn() { 
         bitWrite(SDA_RD,SDA_PIN,0);
         bitWrite(SDA_PORT,SDA_PIN,1);
	 return bitRead(PIND, SDA_PIN); 
	}

void sclHi() { hold(); bitWrite(SCL_PORT,SCL_PIN, 1); }

void sclLo() { hold(); bitWrite(SCL_PORT, SCL_PIN, 0); }


void i2c_init () {
    sdaOut(1);
    bitWrite(SDA_RD,SDA_PIN,1);
    bitWrite(SCL_RD,SCL_PIN,1);
    sclHi();
}

uint8_t i2c_start(uint8_t addr) {
//    sclLo();
   hold();
//    sclHi();
    sdaOut(0);
    return i2c_write(addr);
}

void i2c_stop() {
    sdaOut(0);
    sclHi();
    sdaOut(1);
}

uint8_t i2c_write(uint8_t data)  {
    sclLo();
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        sdaOut(data & mask);
        sclHi();
        sclLo();
    }
    sdaOut(1);
    sclHi();
    uint8_t ack = ! sdaIn();
    sclLo();
    return ack;
}

uint8_t i2c_read(uint8_t last)  {
    uint8_t data = 0;
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        sclHi();
        if (sdaIn())
            data |= mask;
        sclLo();
    }
    sdaOut(last);
    sclHi();
    sclLo();
    if (last)
        i2c_stop();
    sdaOut(1);
    return data;
}

void i2c_send(uint8_t addr){
   i2c_start(addr);
}


void i2c_receive(uint8_t addr){
   i2c_start(addr | 1);
}






