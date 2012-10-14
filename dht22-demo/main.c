// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <uart/uart.h>
#include <avr/delay.h>
#include <dht22/dht22.h>
#define UART_BAUD_RATE      57600 
int main (void) {
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) );
    sei();
    uart_puts("DHT 22 demo\n");

    int temp;
    char buf[10];
    char b[100];

    for(;;){
       uart_puts("ax\r\n");
       
       dht22_readData();
       temp = result.error;
       memset(buf,0,10);
       itoa(temp,buf,16);
       uart_puts(buf);
       uart_puts("\r\n");
       memset(buf,0,10);
       itoa(result.lastTemperature,buf,16);
       uart_puts(buf);


/*       for (int i=0;i<41;i++){

          memset(buf,0,10);
          itoa(bitTimes[i],b,16);
          uart_puts(b);
          uart_puts("\r\n");
       }
       uart_puts("xxx\n");*/

       _delay_ms(5000);
      
    }
}


