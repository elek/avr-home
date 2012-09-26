// Ports library definitions
// 2009-02-13 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include "Ports.h"
#include <avr/sleep.h>
#include <util/atomic.h>



PortI2C::PortI2C ()
{
    sdaOut(1);
    bitWrite(DDRD,7,1);
    bitWrite(DDRC,3,1);
//    mode2(OUTPUT);
    sclHi();
}

uint8_t PortI2C::start(uint8_t addr) const {
    sclLo();
    sclHi();
    sdaOut(0);
    return write(addr);
}

void PortI2C::stop() const {
    sdaOut(0);
    sclHi();
    sdaOut(1);
}

uint8_t PortI2C::write(uint8_t data) const {
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

uint8_t PortI2C::read(uint8_t last) const {
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
        stop();
    sdaOut(1);
    return data;
}


