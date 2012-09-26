#include "ports.h"

void ds1621_startConversion(uint8_t addr); 
void ds1621_init();
int ds1621_getTemp(uint8_t reg);
int ds1621_getHrTemp();



