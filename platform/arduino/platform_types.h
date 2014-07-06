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
// platform-types.h
//
// Defines all platform specific types.
//
// Types that must be defined:
//
// NULL TRUE FALSE uint8_t uint16_t uint32_t int8_t int16_t int32_t Uuint16_t Uuint32_t BOOL SIZE8 SIZE16 PTR
//
// In our case, uint8_t uint16_t uint32_t int8_t int16_t int32_t Uuint16_t Uuint32_t are defined in compiler_defs.h provided by SiLabs
//
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H
#include <stdint.h>
//#include "compiler_defs.h"

#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef char BOOL;

typedef uint16_t PTR;

#ifdef SDCC
#define code __code
#define   __ 
#define uint8_t __bit
#define reentrant __reentrant
#endif


#define PGMPTR uint8_t *
#define PGMMEM code

#endif // PLATFORM_TYPES_H
