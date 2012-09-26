// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include "config.h"
#include "rfm12_config.h"
#include <ds1621/ds1621.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>
#include <uart/uart.h>
#include <rfm12/rfm12.h>
#include <avr/interrupt.h>

#define UART_BAUD_RATE      57600      
#define POWER_SAVE 0



char buf[10];
int i = 0;
ISR(TIMER2_COMPA_vect){
   //wakeup
}

void send_data(){
    buf[0]='7';
    buf[1]='4';
    buf[2]='7';
    buf[3]='4';
    int temp = 0;
    temp = ds1621_getHrTemp();
    memset(buf+4,0,10);
    uint8_t start = 4;
    if (temp > 0xff) {
       buf[start] = '0';
       start++;       
    }
    itoa(temp,buf + start,16);
    uart_puts(buf);
    uart_puts("\r\n");

       
    uint8_t str[10];
    for (int i=0;i<10;i++){
       str[i] = buf[i];
    }
    rfm12_tx(10,0,str);
    rfm12_tick();
 

}

int main () {
    unsigned int c;
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) ); 
    _delay_ms(100);
    rfm12_init();
    sei();
    uart_puts("String stored in SRAM\n");

    ds1621_init();
#if POWER_SAVE
//    TCCR1B |= (1 << WGM12);
//    TIMSK1 |= (1 << OCIE1A);
    TCCR2A |= (1<<WGM21);
    TIMSK2 |= (1 << OCIE2A);
//    OCR1A   = 50; 
    OCR2A = 50;
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
#endif

    for(;;){

#if POWER_SAVE
       _delay_ms(100);
       cli();
       CLKPR = (1 << CLKPCE);
       CLKPR = 8;
       sei();
//       TCNT1H = 0;
//       TCNT1L = 0;
//       TCCR1B |= ((1 << CS12) | (0 << CS11) | (1<< CS10)); // Start timer at Fcpu/1024
       TCNT2 = 0;
       TCCR2B |= ((1 << CS22) | (1 << CS21) | (1<< CS20)); // Start timer at Fcpu/1024

      
       sleep_mode();
       _delay_ms(20);
       
       TCCR2B |= ((0 << CS22) | (0 << CS21) | (0<< CS20)); // Turn off timer
       cli();
       CLKPR = (1 << CLKPCE);
       CLKPR = 0;
       sei();
       _delay_ms(20);
       if (i++ == 3){
          send_data();
          i=0;
       }
       _delay_ms(2000);
#else
       send_data();
       _delay_ms(5000);
#endif


    }
}

  
