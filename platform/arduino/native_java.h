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
// native_java.h
//
// Public API for native class and method implementations.


#ifndef NATIVE_JAVA_IMPL_H
#define NATIVE_JAVA_IMPL_H

//#include "..\..\vmheap.h"
// TODO:  Rename all of these defines to meet our current naming conventions

#define NATIVE_CLASS_BASE           16

// java/lang/Object
#define NATIVE_CLASS_OBJECT         (NATIVE_CLASS_BASE+0)
#define NATIVE_METHOD_INIT          0

// java/lang/System
#define NATIVE_CLASS_SYSTEM         (NATIVE_CLASS_BASE+1)
#define NATIVE_FIELD_OUT            0
#define NATIVE_FIELD_IN             1

// java/io/PrintStream
#define NATIVE_CLASS_PRINTSTREAM    (NATIVE_CLASS_BASE+2)
#define NATIVE_METHOD_PRINTLN_STR   1
#define NATIVE_METHOD_PRINTLN_INT   2
#define NATIVE_METHOD_PRINTLN_CHAR  3
#define NATIVE_METHOD_PRINT_STR     4
#define NATIVE_METHOD_PRINT_INT     5
#define NATIVE_METHOD_PRINT_CHAR    6
#define NATIVE_METHOD_FORMAT        7

// java/io/InputStream
#define NATIVE_CLASS_INPUTSTREAM    (NATIVE_CLASS_BASE+3)
#define NATIVE_METHOD_INPUTSTREAM_AVAILABLE 1
#define NATIVE_METHOD_INPUTSTREAM_READ      2

// java/lang/StringBuffer
#define NATIVE_CLASS_STRINGBUFFER   (NATIVE_CLASS_BASE+4)
#define NATIVE_METHOD_INIT_STR      1
#define NATIVE_METHOD_APPEND_STR    2
#define NATIVE_METHOD_APPEND_INT    3
#define NATIVE_METHOD_APPEND_CHR    4
#define NATIVE_METHOD_TOSTRING      5
#define NATIVE_METHOD_APPEND_FLOAT  6

// nanovm/lang/Math
#define NATIVE_CLASS_MATH           (NATIVE_CLASS_BASE+5)

// nanovm/util/Formatter
#define NATIVE_CLASS_FORMATTER      (NATIVE_CLASS_BASE+6)
#define NATIVE_METHOD_FORMATI		0
#define NATIVE_METHOD_FORMATZ		1
#define NATIVE_METHOD_FORMATF		2

// RFM50
#define NATIVE_CLASS_RFM50          (NATIVE_CLASS_BASE+7)
#define NATIVE_METHOD_LED			1
#define NATIVE_METHOD_WAIT			2

// BRIKPINS
#define NATIVE_CLASS_BRIKPINS		(NATIVE_CLASS_BASE+15)
#define NATIVE_METHOD_READIOSTATE	1
#define NATIVE_METHOD_SETIOSTATE	2
#define NATIVE_METHOD_SETIOFUNCTION	3
#define NATIVE_METHOD_SETPWM		4
#define NATIVE_METHOD_READANALOG	5
#define NATIVE_METHOD_SETTIMEROUTPERIOD	6
#define NATIVE_METHOD_READCOUNTER	7
#define NATIVE_METHOD_RESETCOUNTER	8

//
/*
#define NATIVE_ID(c,m)  ((c<<8)|m)

#define NATIVE_ID2CLASS(m)   (m>>8)
#define NATIVE_ID2METHOD(m)  (m&0xff)
#define NATIVE_ID2FIELD(m)   (m&0xff)
*/

void InvokeNativeMethod(uint16_t mref);
tHeapId CreateNewNativeObject(uint16_t mref);
void NativeInitialize(void);
void WaitMsec(int msec);

#endif // NATIVE_IMPL_H
