
#include <avr/io.h>
#include <util/delay.h>
#include <util.h>

#define LED PA6
#define INP PA7

int main(void)
{
  pinMode(LED,OUTPUT);
  int delay = 1;
  while (1){
     digitalWrite(LED,HIGH);
     for (int i=0;i<delay;i++){
        _delay_ms(1000);
     }
     digitalWrite(LED,LOW);
     for (int i=0;i<delay;i++){
        _delay_ms(1000);
    }
    if (digitalRead(INP) == HIGH) {
       delay = 1;
    } else {
       delay = 2;
    }
     
  }

  return 1;
}
