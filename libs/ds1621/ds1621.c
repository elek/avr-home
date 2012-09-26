#include "ports.h"
#include <util/delay.h>


// DS1621 demo
// -- by Jon McPhalen (www.jonmcphalen.com)
// -- 21 DEC 2007

// SDA pin is Analog4
// SCL pin is Analog5
// DS1621 has A2, A1, and A0 pins connected to GND

// DS1621 Registers & Commands

#define RDTEMP 0xAA // i2c_read temperature register
#define ACCESSTH 0xA1 // access high temperature register
#define ACCESSTL 0xA2 // access low temperature register
#define ACCESSCFG 0xAC // access configuration register
#define RDCNTR 0xA8 // i2c_read counter register
#define RDSLOPE 0xA9 // i2c_read slope register
#define STARTCNV 0xEE // start temperature conversion
#define STOPCNV 0X22 // i2c_stop temperature conversion

// DS1621 configuration bits

#define DONE 0x80 // conversion is done
#define THF 0x40 // high temp flag
#define TLF 0x20// low temp flag
#define NVB 0x10// non-volatile memory is busy
#define POL 0x02 //output polarity (1 = high, 0 = low)
#define ONESHOT 0x01 // 1 = one conversion; 0 = continuous conversion

#define ADDR 0x90

void ds1621_init(){
   i2c_init();
}
void ds1621_setConfig(uint8_t cfg)
    {
        i2c_send(ADDR);
        i2c_write(ACCESSCFG);
        i2c_write(cfg);
        i2c_stop();
        _delay_ms(15); // allow EE i2c_write time to finish
    }

// i2c_read a DS1621 register
uint8_t ds1621_getReg(uint8_t reg)
    {
        i2c_send(ADDR);
        i2c_write(reg); // set register to i2c_read
        i2c_stop();
        i2c_receive(ADDR);
        uint8_t regVal = i2c_read(2);
        return regVal;
    }

// Sets temperature threshold
// -- whole degrees C only
// -- works only with ACCESSTL and ACCESSTH

    void ds1621_setThresh(uint8_t reg, int tC)
    {
        if (reg == ACCESSTL || reg == ACCESSTH) {
            i2c_send(ADDR);
            i2c_write(reg); // select temperature reg
            i2c_write(tC); // set threshold
            i2c_write(0); // clear fractional bit
            i2c_stop();
            _delay_ms(15);
        }
    }

// Start/i2c_stop DS1621 temperature conversion

    void ds1621_startConversion(uint8_t start)
    {
        i2c_send(ADDR);
        if (start > 0)
            i2c_write(STARTCNV);
        else
            i2c_write(STOPCNV);
        i2c_stop();
    }

// i2c_reads temperature or threshold
// -- whole degrees C only
// -- works only with RDTEMP, ACCESSTL, and ACCESSTH

    int ds1621_getTemp(uint8_t reg)
    {
        int tC;

        if (reg == RDTEMP || reg == ACCESSTL || reg == ACCESSTH) {
            uint8_t tVal = ds1621_getReg(reg);
            if (tVal >= 128) { // negative?
                tC = 0xFF00 | tVal;                       // extend sign bits
            }
            else {
                tC = tVal;
            }
            return tC;                                  // return threshold
        }
        return 0;                                     // bad reg, return 0
    }

// i2c_read high resolution temperature

// -- returns temperature in 1/100ths degrees

// -- DS1620 must be in 1-shot mode


    int ds1621_getHrTemp()
    {
        ds1621_startConversion(1);                        // initiate conversion
        uint8_t cfg = 0;
        while (cfg < DONE) {                          // let it finish
            cfg = ds1621_getReg(ACCESSCFG);
        }

        int tHR = ds1621_getTemp(RDTEMP);                   // get whole degrees i2c_reading
        uint8_t cRem = ds1621_getReg(RDCNTR);                  // get counts remaining
        uint8_t slope = ds1621_getReg(RDSLOPE);                // get counts per degree

        if (tHR >= 0)
            tHR = (tHR  * 100 - 25) + ((slope - cRem) * 100 / slope);
        else {
            tHR = -tHR;
            tHR = (25 - tHR * 100) + ((slope - cRem) * 100 / slope);
        }
        return tHR;
    }



