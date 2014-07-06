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
// microapi.h

#ifndef MICROAPI_DEFS_H
#define MICROAPI_DEFS_H
//#include "compiler_defs.h"
#include "hardware_defs.h"                    // requires compiler_defs.h
#include <stdint.h>
// Interrupt types
typedef enum
{
	NIRQ=0
} InterruptTypes;
// Time of day
struct TimeOfDay {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};
// ******************************************************************************************************************************
// *** Public API ***
// Write multiple bytes to SPI
//void WriteSPIMultiple (uint8_t, uint8_t, VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SEG_XDATA));
/*! \fn void InitializeMicroAPI(void)
\brief Initializes the micro to a known state
*/
void InitializeMicroAPI(void);
// Read a byte from the SPI at 'addr'
//uint8_t ReadSPI(uint8_t);
// Write a byte to 'addr' using SPI
//void WriteSPI(uint8_t, uint8_t);
// Disable interrupts
#define DisableInterrupts EA=0
// Enable interrupts
#define EnableInterrupts EA=1
// Get the interrupt pin status for the radio
#define NIRQStatus IRQ
// Gets the status of the SDN pin
#define SDNPin SDN
// This must be defined in the radioapi.  It will be called by the microapi when an interrupt happens that needs to be 
// handled by the radioapi
//extern void HandleInterrupt(InterruptTypes intType, char* intData);
// This must be defined in radioapi.  It will be called once every millisecond by the Timer3 interrupt
//extern void Handle1MsInterrupt();
// This must be defined in radioapi.  It will be called once every second by the RTC interrupt
//extern void Handle1SecInterrupt();

// ******************************************************************************************************************************


// System Clock Definitions

#define SYSCLK_PINT_OSC		0X00
#define SYSCLK_EXT_OSC		0X01
#define SYSCLK_RTC_OSC		0X03
#define SYSCLK_LPWR_OSC		0X04

#define SYSCLK_DIV1			0X00
#define SYSCLK_DIV2			0X01
#define SYSCLK_DIV4			0X02
#define SYSCLK_DIV8			0X03
#define SYSCLK_DIV16		0X04
#define SYSCLK_DIV32		0X05
#define SYSCLK_DIV64		0X06
#define SYSCLK_DIV128		0X07

#define SYSCLK_RDY			0X80
#define SYSCLK_PINT			24500000
#define SYSCLK_EXT			16000000
#define SYSCLK_RTC			24500000
#define SYSCLK_LPWR			24000000
#endif
