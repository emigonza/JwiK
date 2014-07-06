/*
   UART Interface for ATmega328p

   Created October 18, 2013
   Copyright (C) 2013 Patrick Schubert

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "uart.h"
#include "circbuff.h"
#include <stdlib.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

#if U_PARITY == 'N'    // no parity
#define UPM0 0
#elif U_PARITY == 'E'  // even parity
#define UPM0 ((1<<UPM01))
#elif U_PARITY == 'O'  // odd parity
#define UPM0 ((1<<UPM01) | (1<<UPM00))
#else
#error "U_PARITY must be N,E, or O."
#endif

/* 9 bit serial is not implemented yet!!!!
#if U_DATA_BITS == 9
#define UCSZ0 (0x3)
#define 9BIT 1
#else */
#if (U_DATA_BITS > 8) || (U_DATA_BITS < 5)
#error "U_DATA_BITS must be between 5 and 8 (inclusive)."
#endif
#define UCSZ0 (U_DATA_BITS - 5)
#define BIT9 0
//#endif


static volatile CircularBuffer* rx_cb;
static volatile CircularBuffer* tx_cb;

void uart_init(void)
{
    UCSR0B = 0; //turn everything off
    if (!rx_cb){
        rx_cb = cb_init();
    }
    if (!tx_cb){
        tx_cb = cb_init();
    }
    UBRR0 = UBRR_VALUE;
    UCSR0A = 0; //normal xmit rate
    UCSR0B |= (BIT9 << UCSZ02); //#of bits
    UCSR0C = UPM0 | (UCSZ0<<1); //parity and #of bits
    UCSR0B |= (1<<RXCIE0) | (1<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0);
    PORTC = 0x0;
}

void uart_close(void)
{
    //TODO: stop uart
    cb_free(tx_cb);
    cb_free(rx_cb);
}

void uart_putc(char c)
{
    cb_putc(tx_cb,c);
    UCSR0B |= (1<<UDRE0); // turn on interrupt in case it's off
}

char uart_getc(void)
{
    return cb_getc(rx_cb);
}

void uart_puts(char* c, char len)
{
    cb_puts(tx_cb,c,len);
    UCSR0B |= (1<<UDRE0); // turn on interrupt in case it's off
}

char uart_hasRoom(void)
{
    return cb_hasRoom(tx_cb);
}

char uart_hasData(void)
{
    return cb_hasData(rx_cb);
}

void uart_putnum8(unsigned char num)
{
    cb_puts(tx_cb,"char",4);
    cb_putc(tx_cb,'0' + num/100);
    cb_putc(tx_cb,'0' + (num%100)/10);
    cb_putc(tx_cb,'0' + num%10);
    UCSR0B |= (1<<UDRE0); // turn on interrupt in case it's off
}

void uart_putnum16(int num)
{
    char newString[7] = {0,0,0,0,0,0,0};
    itoa(num, newString, 10);
    char length = 0;
    while (length < 6 && newString[length] != 0){
        length++;
    }
    cb_puts(tx_cb,newString,length);
    UCSR0B |= (1<<UDRE0); // turn on interrupt in case it's off
}

ISR(USART_RX_vect)
{
    cb_putc(rx_cb, UDR0);  // store rx byte in circ buff
}

ISR(USART_UDRE_vect)
{
    if (tx_cb->len){           // if data to send
        UDR0 = cb_getc(tx_cb); // send it.
    } else {
        UCSR0B &= ~(1<<UDRE0); // else turn off TX interrupt
    }
}

