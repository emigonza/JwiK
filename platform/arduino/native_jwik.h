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
//  native_math.h
//

#ifndef NATIVE_JWIK_H
#define NATIVE_JWIK_H

typedef enum e_pinDefinitions 
{
	ANALOG0,
	ANALOG1,
	ANALOG2,
	ANALOG3,
	ANALOG4,
	ANALOG5,
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3,
	GPIO4,
	GPIO5,
	GPIO6,
	GPIO7,
	GPIO8,
	GPIO9,
	GPIO10,
	GPIO11,
	GPIO12,
	GPIO13,
	SCL,
	SDA,
	PWM0,
	PWM1,
	PWM2,
	PWM3,
	PWM4,
	PWM5,
	PWM6,
	PWM7,
	TXD,
	RXD,
	SCK,
	MISO,
	MOSI,
	NSS
} tPinDefinitions;

typedef enum e_pinFunctions 
{
	INPUT, 
	OUTPUT, 
	HIZ, 
	ANALOG,
	I2C,
	SPI, 
	PWM, 
	TIMEROUT, 
	COUNTERIN, 
	UART
} tPinFunctions;

typedef struct 
{
	tPinDefinitions PinDefinition;
	tPinFunctions PinFunction;
	uint8_t * PinSFR;
	uint8_t * PinModeSFR;
	uint8_t * PinAnalogSFR;
	uint8_t * PinXBRSFR;
	uint8_t PinXBRMask;
	uint8_t * PinSkipSFR;
	uint8_t PinSkipMask;
	uint8_t PinPosition;

} Pin;



void NativeBrikInvoke(uint8_t mref);

#endif // NATIVE_MATH_H