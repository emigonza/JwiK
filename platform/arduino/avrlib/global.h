
//*****************************************************************************
//
// File Name	: 'global.h'
// Title		: AVR project global include 
// Author		: Pascal Stang
// Created		: 7/12/2001
// Revised		: 9/30/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//	Description : This include file is designed to contain items useful to all
//					code files and projects.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GLOBAL_H
#define GLOBAL_H

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"

#define __AVR_ATmega328P__
#include "avr/io.h"

#define DDRA DDRB
#define DDRE DDRB
#define DDRF DDRB
#define PORTA PORTB
#define PORTE PORTB
#define PORTF PORTB
#define SRE 0
#define TIMSK TIMSK0
#define TCCR0 TCCR0A
#define TCCR2 TCCR2A
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define UBRRL UBRR0L
#define UDR UDR0
#define TXEN TXEN0
#define RXEN RXEN0
#define UCR UCSR0B
#define UCSR1B UCSR0B
#define UBRR1L UBRR0L
#define UCSR1A UCSR0A
#define UDR1 UDR0
#define UDRE UDRE0
#define TICIE1 0

// project/system dependent defines

// CPU clock speed
#define F_CPU        16000000               		// 16MHz processor
//#define F_CPU        14745000               		// 14.745MHz processor
//#define F_CPU        8000000               		// 8MHz processor
//#define F_CPU        7372800               		// 7.37MHz processor
//#define F_CPU        4000000               		// 4MHz processor
//#define F_CPU        3686400               		// 3.69MHz processor
#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#endif
