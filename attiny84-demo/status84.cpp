// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include <util/delay.h>
#include <string.h>
#include "RF12.h"
#include <avr/interrupt.h>
#define UART_BAUD_RATE      57600      


int main () {

    unsigned int c;
//    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) ); 
    _delay_ms(100);
//    rfm12_init();
    rf12_initialize(25, RF12_868MHZ, 5);    
    sei();
//    uart_puts("Master node is starting...\r\n");
    uint16_t status;
    char buffer[10];

    for(;;){
//       status = rfm12_status();
//       itoa(status,&buffer,16);
//       uart_puts(buffer);
//       uart_puts("\r\n");
       _delay_ms(1000);
       

   }
}

  
