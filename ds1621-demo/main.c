// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include <ds1621/ds1621.h>
#include <avr/interrupt.h>
#include <uart/uart.h>
#include <avr/delay.h>

#define UART_BAUD_RATE      57600 
int main (void) {
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) );
    sei();
    uart_puts("String stored in SRAM\n");
    ds1621_init();

    int temp;
    char buf[10];

    for(;;){
       uart_puts("ax\r\n");
       temp = ds1621_getHrTemp();
       memset(buf,0,10);
       itoa(temp,buf,16);
       uart_puts(buf);

       _delay_ms(5000);
      
    }
}


