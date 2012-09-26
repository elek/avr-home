#include "Ports.h"
#include <util/delay.h>


// DS1621 demo
// -- by Jon McPhalen (www.jonmcphalen.com)
// -- 21 DEC 2007

// SDA pin is Analog4
// SCL pin is Analog5
// DS1621 has A2, A1, and A0 pins connected to GND

// DS1621 Registers & Commands

#define RDTEMP 0xAA // read temperature register
#define ACCESSTH 0xA1 // access high temperature register
#define ACCESSTL 0xA2 // access low temperature register
#define ACCESSCFG 0xAC // access configuration register
#define RDCNTR 0xA8 // read counter register
#define RDSLOPE 0xA9 // read slope register
#define STARTCNV 0xEE // start temperature conversion
#define STOPCNV 0X22 // stop temperature conversion

// DS1621 configuration bits

#define DONE 0X80// conversion is done
#define THF 0x40 // high temp flag
#define TLF 0x20// low temp flag
#define NVB 0x10 // non-volatile memory is busy
#define POL 0x02 //output polarity (1 = high, 0 = low)
#define ONESHOT 0x01;/0/= one conversion; 0 = continuous conversion

class TempSensor : public DeviceI2C {
// Set configuration register
public:
    TempSensor(PortI2C& port) : DeviceI2C(port, 0x48) {

    }

    void setConfig(uint8_t cfg)
    {
        send();
        write(ACCESSCFG);
        write(cfg);
        stop();
        _delay_ms(15); // allow EE write time to finish
    }

// Read a DS1621 register

    uint8_t getReg(uint8_t reg)
    {
        send();
        write(reg); // set register to read
        stop();
        receive();
        uint8_t regVal = read(2);
        return regVal;
    }

// Sets temperature threshold
// -- whole degrees C only
// -- works only with ACCESSTL and ACCESSTH

    void setThresh(uint8_t reg, int tC)
    {
        if (reg == ACCESSTL || reg == ACCESSTH) {
            send();
            write(reg); // select temperature reg
            write(uint8_t(tC)); // set threshold
            write(0); // clear fractional bit
            stop();
            _delay_ms(15);
        }
    }

// Start/Stop DS1621 temperature conversion

    void startConversion(uint8_t start)
    {
        send();
        if (start > 0)
            write(STARTCNV);
        else
            write(STOPCNV);
        stop();
    }

// Reads temperature or threshold
// -- whole degrees C only
// -- works only with RDTEMP, ACCESSTL, and ACCESSTH

    int getTemp(uint8_t reg)
    {
        int tC;

        if (reg == RDTEMP || reg == ACCESSTL || reg == ACCESSTH) {
            uint8_t tVal = getReg(reg);
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

// Read high resolution temperature

// -- returns temperature in 1/100ths degrees

// -- DS1620 must be in 1-shot mode


    int getHrTemp()
    {
        startConversion(true);                        // initiate conversion
        uint8_t cfg = 0;
        while (cfg < 0x80) {                          // let it finish
            cfg = getReg(ACCESSCFG);
            _delay_us(10);
        }

        int tHR = getTemp(RDTEMP);                   // get whole degrees reading
        uint8_t cRem = getReg(RDCNTR);                  // get counts remaining
        uint8_t slope = getReg(RDSLOPE);                // get counts per degree

        if (tHR >= 0)
            tHR = (tHR  * 100 - 25) + ((slope - cRem) * 100 / slope);
        else {
            tHR = -tHR;
            tHR = (25 - tHR * 100) + ((slope - cRem) * 100 / slope);
        }
        return tHR;
    }

};



