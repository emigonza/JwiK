// Java VM for small microcontrollers
//
// (c) 2012, Digital Six Laboratories LLC
// All Rights Reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial 
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Original work based on NanoVM (http://www.harbaum.org/till/nanovm/index.shtml) for AVR microcontroller
//=========================================================================================================
// 
// uart.h
//
// Si1000 platform UART api

#ifndef UART_DEFS_H
#define UART_DEFS_H
//#include "compiler_defs.h"
#include "hardware_defs.h"                    

// Change this to change the buffer size.  Needs to be a power of 2
#define BUFFER_SIZE 64
// This mask is used to force the pointers to wrap at the end of the buffer.
#define BUFFER_MASK BUFFER_SIZE-1
// Defines all supported baud rates
typedef enum 
{
	b1200,
	b2400,
	b9600,
	b14400,
	b28800,
	b57600,
	b115200,
	b230400
} UartBaudRates;
// ** uint8_t InitializeUart(UartBaudRates baudRate)
//
// Call this function to set the UART baud rate.  The possible baud rates are 
// defined in the UartBaudRates enumeration.

uint8_t InitializeUart( UartBaudRates baudRate );
// ** void UartFlush(void) **
//
// Empty both buffers by setting both head and tail pointers to the start of the buffer.

void UartFlush(void);
// ** uint8_t UartReadByte(void) **
//
// Reads a byte, if available, from the receive buffer.  If a byte is not available, this function
// will return a 0.

uint8_t UartReadByte(void);
// ** uint16_t UartBytesAvailable(void) **
//
// Returns the number of bytes available in the receiver buffer.

uint16_t UartBytesAvailable(void);
// ** void UartSendByte(uint8_t b2s) **
//
// Stuff a byte in the transmit UART buffer and advance the head pointer.  If the head and tail
// pointer are equal, that means that the UART interrupts are not working because the buffer
// was empty.  In that case, we need to force the interrupt to happen so that the first byte will
// be sent.

void UartSendByte(uint8_t b2s);

#endif
