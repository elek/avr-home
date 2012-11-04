#include "hardsleep.h"
#include <avr/sleep.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define HARDSLEEP_SLOWDOWN 1
void hardsleep_init(){
    TCCR2B |= ((0 << CS22) | (0 << CS21) | (0<< CS20)); // Turn off timer

    //Clear timer on compare match
    TCCR2A |= (1<<WGM21);

    //enable interrupt
    TIMSK2 |= (1 << OCIE2A);

//    OCR1A   = 50; 
    OCR2A = 255;
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
}

ISR(TIMER2_COMPA_vect){
   //wakeup
}

void hardsleep_sleep(uint8_t counter){
#if HARDSLEEP_SLOWDOWN
//slow down
       cli();
       CLKPR = (1 << CLKPCE);
       CLKPR = 8; // clk/256
       sei();

#endif
//start counter
          TCNT2 = 0;
          //clk/1024
          TCCR2B |= ((1 << CS22) | (1 << CS21) | (1<< CS20)); 
//sleep
      
       for (int i=0;i<counter;i++) {
          sleep_mode();
       }
       TCCR2B |= ((0 << CS22) | (0 << CS21) | (0<< CS20)); // Turn off timer
#if HARDSLEEP_SLOWDOWN
       //speed up processor
       cli();
       CLKPR = (1 << CLKPCE);
       CLKPR = 0; //clk/1
       sei();
#endif
       _delay_ms(20);

}
