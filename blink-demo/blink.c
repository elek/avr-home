
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  // Set Port to OUTPUT
  DDRD = 0xff;
  while (1){
     PORTD = 0xFF;
     _delay_ms(1000);
     PORTD = 0x00;
     _delay_ms(1000);
  }
  // Set all Port B pins as HIGH

  return 1;
}
