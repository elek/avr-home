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
#include <uart/uart.h>
#include "config.h"
#include "rfm12_config.h"
#include <rfm12/rfm12.h>
#include <avr/interrupt.h>
#define UART_BAUD_RATE      57600      


int main () {

    unsigned int c;
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) ); 
    _delay_ms(100);
    rfm12_init();
    
    sei();
    uart_puts("Master node is starting...'\r\n");

    char buf[10];
    uint8_t *bufptr;
    for(;;){

 	if (rfm12_rx_status() == STATUS_COMPLETE)
                {
                        //so we have received a message


                        bufptr = rfm12_rx_buffer(); //get the address of the current rx buffer

                        // dump buffer contents to uart                 
                        for (int i=0;i<rfm12_rx_len();i++)
                        {
                                uart_putc ( bufptr[i] );
                        }
                        
                        uart_puts ("\r\n");
                        
                        // tell the implementation that the buffer
                        // can be reused for the next data.
                        rfm12_rx_clear();
                }

       for (int i=0;i<20;i++){
          rfm12_tick();
          _delay_ms(100);
       }
       

   }
}

  
