// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include "vuart.h"
#include <avr/delay.h>
#define UART_BAUD_RATE      57600 

int main () {
    vuart_init();
    for(;;){
       vuart_write_data(60);
       _delay_ms(1000);
    }
}


