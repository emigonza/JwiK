//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005 by Till Harbaum <Till@Harbaum.org>
//
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
// vmarray.h

#ifndef VMARRAY_H
#define VMARRAY_H

#include "vmheap.h"

// Array element type definitions

typedef enum 
{ 
	kBoolean=4, 
	kChar, 
	kFloat,
	kDouble,
	kByte,
	kShort,
	kInt,
	kLong
} ArrayElementTypes;


tHeapId   ArrayNew(tVmInt length, uint8_t type);
tVmInt   ArrayLen(tHeapId id);
void	    ArraySetByteValue(tHeapId id, tVmInt index, tVmByte value);
tVmByte  ArrayGetByteValue(tHeapId id, tVmInt index);
void        ArraySetIntValue(tHeapId id, tVmInt index, tVmInt value);
tVmInt   ArrayGetIntValue(tHeapId id, tVmInt index);
#ifdef VM_INCLUDE_FLOAT
void        ArraySetFloatValue(tHeapId id, tVmInt index, tVmFloat value);
tVmFloat ArrayGetFloatValue(tHeapId id, tVmInt index);
#endif

#endif // VMARRAY_H
