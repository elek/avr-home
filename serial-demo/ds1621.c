#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <uart/uart.h>
#include <i2cmaster.h>

#define UART_BAUD_RATE      57600      


#define DEV_TYPE   0x90                    // shift required by wire.h
#define DEV_ADDR   0x00                         // DS1621 address is 0
#define SLAVE_ID   DEV_TYPE | DEV_ADDR

// DS1621 Registers & Commands

#define RD_TEMP    0xAA                         // read temperature register
#define ACCESS_TH  0xA1                         // access high temperature register
#define ACCESS_TL  0xA2                         // access low temperature register
#define ACCESS_CFG 0xAC                         // access configuration register
#define RD_CNTR    0xA8                         // read counter register
#define RD_SLOPE   0xA9                         // read slope register
#define START_CNV  0xEE                         // start temperature conversion
#define STOP_CNV   0X22                         // stop temperature conversion

// DS1621 configuration bits

#define DONE       B10000000                    // conversion is done
#define THF        B01000000                    // high temp flag
#define TLF        B00100000                    // low temp flag
#define NVB        B00010000                    // non-volatile memory is busy
#define POL        0x02 //10                    // output polarity (1 = high, 0 = low)
#define ONE_SHOT   0x01 //01                    // 1 = one conversion; 0 = continuous conversion



uint8_t getReg(uint8_t reg) {
 uint8_t res;
 i2c_start_wait(SLAVE_ID + I2C_WRITE);
 i2c_write(reg);
 i2c_rep_start(SLAVE_ID + I2C_READ);
 res = i2c_readNak();
 i2c_stop();
 return res;
}

int main(void)
{
    unsigned int c;
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
    
    sei();
    
    uart_puts("String stored in SRAM\n");
    
    uart_puts_P("String stored in FLASH\n");
    
    unsigned char ret;

    i2c_init(); 
    ret = i2c_start(SLAVE_ID + I2C_WRITE);
    if (ret){
       uart_puts_P("Error on i2c init");
    }
    i2c_write(STOP_CNV);
    i2c_stop();
    _delay_ms(20);
    uart_puts_P("i2c initialized\r\n");


    
    i2c_start_wait(SLAVE_ID + I2C_WRITE);
    i2c_write(ACCESS_CFG);
    i2c_write(POL | ONE_SHOT);
    i2c_stop();

    uart_puts_P("i2c configured\r\n");
    _delay_ms(20);

    char buf[10];
    uint8_t res = getReg(ACCESS_CFG);
    itoa(res,buf,10);
    uart_puts(buf);
    for(;;)
    {
   	i2c_start_wait(SLAVE_ID + I2C_WRITE);
    	i2c_write(START_CNV);
        i2c_stop();


        uart_puts_P("conversion started");

	int tC;
   	uint8_t tVal = getReg(RD_TEMP);


        uart_puts_P("value retrieved");

	if (tVal > 0XFF) { 
     	   tC = 0xFF00 | tVal;  
	}
	 else {
     	   tC = tVal;
   	}
	itoa(tC,buf,10);
        uart_puts(buf);
        uart_puts_P("\r\n");
       _delay_ms(1000);
    }
    
}
