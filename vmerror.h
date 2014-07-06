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
// vmerror.h

#ifndef VMERROR_H
#define VMERROR_H
#include "vmtypes.h"

// enumeration of error types
typedef enum 
{
	kErrorHeapBase,
	kErrorHeapIllegalChunkSize,     
	kErrorHeapCorrupted,              
	kErrorHeapOutOfMemory,          
	kErrorHeapChunkMissing,   
	kErrorHeapOutOfStackMemory,    
	kErrorHeapStackUnderrun,         
	kErrorArrayBase,                  
	kErrorArrayIllegalType,         
	kErrorNativeBase,                 
	kErrorNativeUnknownMethod,       
	kErrorNativeUnknownClass,        
	kErrorNativeIllegalArgument,     
	kErrorVmFileBase,                
	kErrorVmFileBadJuJu,               
	kErrorVmFileBadVersion,             
	kErrorVmBase,                     
	kErrorVmIllegalReference,        
	kErrorVmUnsupportedOpCode,       
	kErrorVmDivisionByZero,         
	kErrorVmStackCorrupted          
} tErrorTypes;

void HandleError(tErrorTypes errcode);

#endif // VMERROR_H
