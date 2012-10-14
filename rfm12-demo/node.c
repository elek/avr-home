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

void send_data(){
    buf[0]='7';
    buf[1]='4';
    buf[2]='7';
    buf[3]='4';
    uart_puts("sending \r\n");
    rfm12_tx(10,0,buf);
    for (int i=0;i<10;i++){
       rfm12_tick();
       _delay_ms(2);
    }
 

}

int main () {
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) ); 
    _delay_ms(100);
    rfm12_init();
    sei();
    uart_puts("String stored in SRAM\n");


    for(;;){

       send_data();
       _delay_ms(5000);

    }
}

 
