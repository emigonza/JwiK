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
// microapi.c

#include "microapi.h"
#include <stdint.h>

// Global Variables
uint32_t sysclk;
uint8_t brtcupdate = 0;
uint8_t brtcset = 0;
uint32_t rtccapture;
struct TimeOfDay rtc;

// ***********************************************************************************
// *** Interrupt Handlers ***
/*
INTERRUPT( TIMER3_ISR, INTERRUPT_TIMER3 )
{
	TMR3CN	&= ~0x80;
	Handle1MsInterrupt();
	//jms++;
//	P2 ^= 0x20;
}	
*/
// ***********************************************************************************
// *** Internal functions ***
/*
void InitializePorts( void )
{
    uint16_t delay;

     Init LPOSC to 10 MHZ clock for now.
    CLKSEL      = 0x14;

    XBR0        |= 0x01;            // Enable UART
    P0MDOUT     |= 0x10;            // UART TX Push-Pull
    P0SKIP      |= 0xCF;            // skip P0.0-3 & 0.6-7

    XBR1        |= 0x40;            // Enable SPI1 (3 wire mode)
    P1MDOUT     |= 0x01;            // Enable SCK push pull
    P1MDOUT     |= 0x04;            // Enable MOSI push pull
    P1SKIP      |= 0x08;            // skip P1.3
    P1SKIP      |= 0x10;            // skip NSS
    P1MDOUT     |= 0x10;            // Enable NSS push pull

	P1SKIP		|= 0x60;			// PB on RFm50 board

    
	//P1SKIP      |= 0x60;            // skip LEDs
	P2SKIP		|= 0x11;
	P2MDOUT		|= 0x11;
		
    P2MDOUT     |= 0x40;            // SDN push pull

    SFRPAGE     = CONFIG_PAGE;
    P0DRV       = 0x10;             // TX high current mode
    P1DRV       = 0x75;             // MOSI, SCK, NSS, LEDs high current mode
    P2DRV       = 0x40;             // SDN high current
    SFRPAGE     = LEGACY_PAGE;

    XBR2        |= 0x40;            // enable Crossbar

    SDN         = 0;

    // SND delay (3907/156250=25 ms)
    CLKSEL      = 0x74;
    delay       = 3907;
    do{}while (--delay);
    CLKSEL      = 0x14;



	PMU0CF = 0x20;
	P0MDIN	&= ~0x0c;
	P0MDOUT	|= 0x10;
	P1MDOUT	|= 0x15;
	P2MDIN = 0xff;
	P2MDOUT = 0x0e;

	SFRPAGE   = CONFIG_PAGE;
	P1DRV     = 0x15;
	P2DRV     = 0x40;
	SFRPAGE   = LEGACY_PAGE;

	XBR0    |= 0x01;
	XBR1    |= 0x40;
	XBR2    |= 0x40;

	IT01CF	= 0x07;
	IE0	= 0;
	IT0	=0;

}
*/
/*
void InitializeSPI1( void )
{

	SPI1CFG = 0x40;
	SPI1CN = 0x00;
	SPI1CKR = 0x00; //SPI_CKR_VALUE;
	SPI1CN |= 0x01;
	NSS1 = 1;

}
*/
/*
void InitializeTimer3( void )

	CKCON	|= 0x40;
	TMR3RLL	= 65536 - sysclk / 1000;
	TMR3RLH = ( 65536 - sysclk / 1000 ) >> 8;
	TMR3L	= TMR3RLL;
	TMR3H	= TMR3RLH;
	TMR3CN	= 04;
	EIE1	|= 0x80;   	

}
*/
// initialize RTC.  Interrupt every rtc_alarm/32768 seconds.
/*
void InitializeRTC( uint32_t rtc_alarm )
{

	if ( RTC0KEY == 0x00 ) {
		RTC0KEY = 0xa5;
		RTC0KEY = 0xf1;
	}
	if ( RTC0KEY == 0x02 ) {
		RTC0ADR = 0x17;
		RTC0DAT = 0x67;
		NOP();
		RTC0ADR = 0x15;
		RTC0DAT = 0x60;
		NOP();
		RTC0ADR = 0x16;
		RTC0DAT = 0x14;
		NOP();
		RTC0ADR = 0x14;
		RTC0DAT = 0x80;
		NOP();
		while ( 1 ) {
			RTC0ADR = 0xd5;
			NOP();
			if ( RTC0DAT & 0x10 ) {
				RTC0ADR = 0xd6;
				NOP();
				if ( RTC0DAT & 0x40 )
					break;
			}
			
		}
		RTC0ADR = 0x12;
		RTC0DAT = 0x00;
		NOP();
		RTC0DAT = 0x00;
		NOP();
		RTC0ADR = 0x15;
		RTC0DAT = 0xc0;
		NOP();
		RTC0ADR = 0x18;
		RTC0DAT = rtc_alarm;
		NOP();
		RTC0DAT = ( rtc_alarm >> 8 );
		NOP();
		RTC0DAT = ( rtc_alarm >> 16 );
		NOP();
		RTC0DAT = ( rtc_alarm >> 24 );
		NOP();
		RTC0ADR = 0x14;
		RTC0DAT = 0xde;
		NOP();

	}
	rtc.hour = 12;
	rtc.minute = 0;
	rtc.second = 0;
	EIE1 |= 0x02;

}
*/
/*
uint32_t CalculateSystemClock( uint8_t clksource, uint8_t clkdivider, uint8_t bslowtofast )
{

	uint32_t clk;

	if ( clksource == SYSCLK_PINT_OSC ) {
		OSCICN = 0x8f;
		while ( !(OSCICN & 0x40) )	;
		clk = SYSCLK_PINT / ( 0x01 << clkdivider );
	}
	else if ( clksource == SYSCLK_EXT_OSC ) {
		OSCXCN = 0x67;
		while ( !(OSCXCN & 0x80) )	;
		clk = SYSCLK_EXT / ( 0x01 << clkdivider );
	}
	if ( clksource == SYSCLK_RTC_OSC )
		clk = SYSCLK_RTC / ( 0x01 << clkdivider );
	else if ( clksource == SYSCLK_LPWR_OSC )
		clk = SYSCLK_LPWR / ( 0x01 << clkdivider );
	if ( bslowtofast ) {
		CLKSEL = clksource;
		CLKSEL += ( clkdivider << 4 );
		while ( !(CLKSEL & SYSCLK_RDY) )	;
	}
	else {
		CLKSEL = ( clkdivider << 4 );
		while ( !(CLKSEL & SYSCLK_RDY) )	;
		CLKSEL += clksource;
	}
	if ( clksource == SYSCLK_PINT_OSC ) {
		OSCXCN = 0x00;
	}
	else if ( clksource == SYSCLK_EXT_OSC ) {
		OSCICN = 0x0f;
	}
	else {
		OSCICN = 0x0f;
		OSCXCN = 0x00;
	}
	FLSCL = 0x40;
	return ( clk );
}
*/
/*
void InterruptInit(void)
{

	IT01CF	= 0x07;								// INT0 mapped to P0.
	
    TCON &= ~0x03;                      // clear IE0 & IT0
	// make int0 edge triggered
	TCON |=1;

}
*/
// ***********************************************************************************
// *** Public API ***

void InitializeMicroAPI(void)
{

}
/*
uint8_t ReadSPI(uint8_t reg) reentrant
{

	uint8_t value;
   uint8_t restoreInts;

   // Disable MAC interrupts
   restoreInts = IE & 0x03;             // save EX0 & ET0 state
   IE &= ~0x03; 

   // Send SPI data using double buffered write
   NSS1 = 0;                           // dsrive NSS low
   SPIF1 = 0;                          // cleat SPIF
   SPI1DAT = ( reg );                  // write reg address
   while(!TXBMT1);                     // wait on TXBMT
   SPI1DAT = 0x00;                     // write anything
   while(!TXBMT1);                     // wait on TXBMT
   while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
   value = SPI1DAT;                    // read value
   SPIF1 = 0;                          // leave SPIF cleared
   NSS1 = 1;                           // drive NSS low

   // Restore MAC interrupts
   IE |= restoreInts;                  // restore EX0 & ET0


   return value;

}
*/
/*
void WriteSPI(uint8_t reg, uint8_t value) reentrant
{

	uint8_t restoreEA;

   // disable interrupts during SPI transfer
   restoreEA = EA;
   EA = 0;

   // Send SPI data using double buffered write
   NSS1 = 0;                           // drive NSS low
   SPIF1 = 0;                          // clear SPIF
   SPI1DAT = (reg | 0x80);             // write reg address
   while(!TXBMT1);                     // wait on TXBMT
   SPI1DAT = value;                    // write value
   while(!TXBMT1);                     // wait on TXBMT
   while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY

   SPIF1 = 0;                          // leave SPIF cleared
   NSS1 = 1;                           // drive NSS high

   // Restore interrupts after SPI transfer
   EA = restoreEA;

}
*/
/*
void WriteSPIMultiple (uint8_t count, uint8_t reg,  VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SEG_XDATA)) reentrant
{

	uint8_t restoreEA;

   // disable interrupts during SPI transfer
   restoreEA = EA;
   EA = 0;

   NSS1 = 0;                            // drive NSS low
   SPIF1 = 0;                           // clear SPIF
   SPI1DAT = (0x80 | reg);

   while(count--)
   {
      while(!TXBMT1);                   // wait on TXBMT
      SPI1DAT = *buffer++;             // write buffer
   }

   while(!TXBMT1);                      // wait on TXBMT
   while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY

   SPIF1 = 0;                           // leave SPI  cleared
   NSS1 = 1;                            // drive NSS high

   // Restore interrupts after SPI transfer
   EA = restoreEA;

}
*/
