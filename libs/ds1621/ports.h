#include <avr/sleep.h>

void i2c_init ();
uint8_t i2c_start(uint8_t addr);
void i2c_stop();
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t last);
