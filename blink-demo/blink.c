
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  // Set Port to OUTPUT
  DDRA = 0x80;
  while (1){
     PORTA = 0x80;
     _delay_ms(1000);
     PORTA = 0x00;
     _delay_ms(1000);
  }
  // Set all Port B pins as HIGH

  return 1;
}
