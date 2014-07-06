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
// native_BriK.c
//
// Native Java BriK Object and Methods


#include "platform_types.h"
#include "config.h"
#include "vmerror.h"
#include "vmstack.h"
#include "vm.h"
#include "native_jwik.h"
#include "native_java.h"

  Pin BrikPins[] = 
{
	// Port 0 pin definitions
	//
	// Pin     Func    SFR     Out SFR      Analog SFR  XBR SFR   XBR MSK   SKPI SFR    SKIP MSK	Pin Pos

	//Port 1 pin definitions

	// Port 2 pin definitions
};
void NativeBrikInvoke(uint8_t mref)
{
	tVmInt op1, op2, op3;
	
	switch(mref)
	{
		case NATIVE_METHOD_READIOSTATE:
			// op1 contains the pin number
			op1 = StackPopInt();
			if(BrikPins[op1].PinFunction == INPUT)
				StackPush( (*((uint8_t*)BrikPins[op1].PinSFR) & BrikPins[op1].PinPosition) > 0);
			else
				StackPush(0);
			break;
		case NATIVE_METHOD_SETIOSTATE:
			// op2 contains pin state
			op2 = StackPopInt();
			// op1 contains pin number
			op1 = StackPopInt();

			if(BrikPins[op1].PinFunction == OUTPUT)
			{			
				if(op2==1)
					*((uint8_t*)BrikPins[op1].PinSFR) |= BrikPins[op1].PinPosition;
				else
					*((uint8_t*)BrikPins[op1].PinSFR) &= ~BrikPins[op1].PinPosition;
			}
			break;
		case NATIVE_METHOD_SETIOFUNCTION:
			// op2 contains desired pin function
			op2 = StackPopInt();
			// op1 contains pin number
			op1 = StackPopInt();
			switch(op2)
			{
				case INPUT:
					if ((op1>=GPIO0) && (op1<=GPIO13))
					{
						BrikPins[op1].PinFunction = INPUT;
						*((uint8_t*)BrikPins[op1].PinModeSFR) &= ~BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinAnalogSFR) &= ~BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) |= BrikPins[op1].PinSkipMask;
					}
					break;
				case OUTPUT:
					if ((op1>=GPIO0) && (op1<=GPIO13))
					{
						BrikPins[op1].PinFunction = OUTPUT;
						*((uint8_t*)BrikPins[op1].PinModeSFR) |= BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinAnalogSFR) &= ~BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) |= BrikPins[op1].PinSkipMask;
					}
					break;
				case HIZ:				
					if ((op1>=GPIO0) && (op1<=GPIO13))
					{
						BrikPins[op1].PinFunction = INPUT;
						*((uint8_t*)BrikPins[op1].PinModeSFR) &= ~BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinAnalogSFR) &= ~BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) |= BrikPins[op1].PinSkipMask;
					}
					break;
				case ANALOG:
					if ((op1>=ANALOG0) && (op1<=ANALOG5))
					{
						BrikPins[op1].PinFunction = ANALOG;
						*((uint8_t*)BrikPins[op1].PinAnalogSFR) |= BrikPins[op1].PinPosition;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) |= BrikPins[op1].PinSkipMask;
					}
					break;
				case I2C:
					// only certain pins can be assigned to I2C.
					// pin 0.6 can be assigned SDA and pin 0.7 can be assigned SCL
					// any other pins will be ignored
					if( (op1>=SCL) && (op1<=SDA) )
					{
						BrikPins[op1].PinFunction = I2C;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) &= ~BrikPins[op1].PinSkipMask;
						*((uint8_t*)BrikPins[op1].PinXBRSFR) |= BrikPins[op1].PinXBRMask;						 
					}
					break;
				case PWM:
					if( (op1>=PWM0) && (op1<=PWM5) )
					{
						BrikPins[op1].PinFunction = PWM;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) &= ~BrikPins[op1].PinSkipMask;
						*((uint8_t*)BrikPins[op1].PinXBRSFR) |= BrikPins[op1].PinXBRMask;
					}
					break;
				case TIMEROUT:
				case COUNTERIN:
				case SPI:
					if( (op1>=SCK) && (op1<=NSS) )
					{
						BrikPins[op1].PinFunction = SPI;
						*((uint8_t*)BrikPins[op1].PinSkipSFR) &= ~BrikPins[op1].PinSkipMask;
						*((uint8_t*)BrikPins[op1].PinXBRSFR) |= BrikPins[op1].PinXBRMask;
					}
					break;
			}
			break;
		case NATIVE_METHOD_SETPWM:
			// op2 contains PWM value
			op2 = StackPopInt();
			// op1 contains pin number
			op1 = StackPopInt();
			

			break;
		case NATIVE_METHOD_READANALOG:
			break;
		case NATIVE_METHOD_SETTIMEROUTPERIOD:
			op3 = StackPopInt();
			// op2 contains pin state
			op2 = StackPopInt();
			// op1 contains pin number
			op1 = StackPopInt();
			break;
		case NATIVE_METHOD_READCOUNTER:
			// op1 contains the pin number
			op1 = StackPopInt();
			break;
		case NATIVE_METHOD_RESETCOUNTER:
			// op1 contains the pin number
			op1 = StackPopInt();
			break;
	}
}
