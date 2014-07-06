/*
   UART Interface for ATmega328p

   Creates a helpful interface for UART. Incoming and outgoing bytes are stored
   in a buffer, allowing the main thread to send and receive bytes and strings
   at its convenience.

   uart_init() starts the uart and initializes the buffer. This must be called
   prior to any other uart calls.

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

#ifndef UART_H
#define UART_H

#include "global.h"

// What baud rate to use
#define BAUD 9600

// How many data bits to send. Currently supports 5-8 bits
#define U_DATA_BITS 8

// Parity selection. Options are 'O' - odd parity
//                               'E' - even parity
//                               'N' - no parity
#define U_PARITY 'N'


// Starts up uart machinery and creates buffers. MUST be called prior
// to any other uart command!
void uart_init(void);

// Closes down uart machinery and deallocates buffers.
void uart_close(void);

// Send a char over uart
void uart_putc(char c);

// Get a char from uart rx buffer
char uart_getc(void);

// Send an entire string over uart
void uart_puts(char* c, char len);

// Returns 1 if outbound buffer has room for more data. 0 if not.
char uart_hasRoom(void);

// Returns 1 if rx buffer has data to be read. 0 if not.
char uart_hasData(void);


void uart_putnum16(int num);
void uart_putnum8(unsigned char num);
#endif
