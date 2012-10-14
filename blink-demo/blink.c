
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  // Set Port to OUTPUT
  DDRD = 0x80;
  while (1){
     PORTD = 0x80;
     _delay_us(10000);
     PORTD = 0x00;
     _delay_ms(1000);
  }
  // Set all Port B pins as HIGH

  return 1;
}
