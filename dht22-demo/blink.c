
#include <avr/io.h>
#include <util/delay.h>


#define DIRECT_READ(base, mask)         (((*(base + 0)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*(base+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*(base+1)) |= (mask))

#define DIRECT_WRITE_LOW(base, mask)    ((*(base + 2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)    ((*(base + 2)) |= (mask))

uint8_t PIN = 0x80;
uint8_t *REG asm("r30") = PIND;

int main(void)
{
  // Set Port to OUTPUT
  DIRECT_MODE_OUTPUT(REG,PIN);
  while (1){
     DIRECT_WRITE_LOW(REG,PIN);
     _delay_ms(1000);
     DIRECT_WRITE_HIGH(REG,PIN);
     _delay_ms(1000);
  }
  // Set all Port B pins as HIGH

  return 1;
}
