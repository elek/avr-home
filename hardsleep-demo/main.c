// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include <avr/interrupt.h>
#include <uart/uart.h>
#include <avr/delay.h>
#include <hardsleep/hardsleep.h>

#define UART_BAUD_RATE      57600 

#define WAIT_AFTER_UART    _delay_ms(500);
int main () {
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) );
    DDRD = 0xFF;
    sei();
    uart_puts("Starting hardsleep demo\r\n");
    WAIT_AFTER_UART    
    hardsleep_init();
    uart_puts("Hardsleep has been initialized\r\n");
    WAIT_AFTER_UART

    for(int i = 0;i<200;i++){
       uart_puts("x\r\n");
       WAIT_AFTER_UART;       
       hardsleep_sleep();
//       _delay_ms(3000);
    }
}


