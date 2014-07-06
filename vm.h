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
// vm.h
//
// Virtual machine source code header file

#ifndef VM_H
#define VM_H
// select the proper configuration file.  This has to be done outside the ifdef because the ifdef is based on settings in the
// config file
//#include "platform\si1000\config.h"
#include "config.h"
/* #ifdef SI1000

#include "platform\si1000\hardware_defs.h"
#include "platform\si1000\si1000_defs.h"
#include "platform\si1000\platform.h"
#endif
*/
#ifdef JWIK
#include "hardware_defs.h"
//#include "platform\jwik\si1000_defs.h"
#include "platform.h"

#endif
//#include "nvmtypes.h"
#include "vmheap.h"
#include "vmfile.h"



// additional items to be allocated on heap during constructor call
#define VM_CLASS_CONST_ALLOC  1

#ifdef VM_INCLUDE_16BIT_WORD

// Basic VM Types
typedef uint16_t tVmWord;
typedef uint16_t tVmStack;
typedef uint16_t tVmReference;
typedef int16_t tVmInt;
typedef uint16_t tVmUInt; 
typedef int16_t tVmShort;
typedef int8_t tVmByte;
typedef union
{
	tVmByte  b[2];
	tVmShort s[1];
	tVmInt   i[1];
} tVmUnion;
// Masks
#define VM_IMMEDIATE_MASK  ((tVmReference)0x8000)
#define VM_TYPE_MASK       ((tVmReference)0xc000)
#define VM_TYPE_HEAP       ((tVmReference)0x8000)
#define VM_TYPE_CONST      ((tVmReference)0xc000)

#endif


#ifdef VM_INCLUDE_32BIT_WORD
// THE basic types
typedef uint32_t tVmWord;
typedef uint32_t tVmStack;
typedef uint32_t tVmReference;
typedef int32_t tVmInt;
typedef uint32_t tVmUInt; // needed for >>> operator
typedef int16_t tVmShort;
typedef uint16_t tVmUShort;
typedef int8_t tVmByte;
# ifdef VM_INCLUDE_FLOAT
typedef float tVmFloat;
# endif
typedef union
{
  	tVmByte  b[4];
  	tVmShort s[2];
  	tVmInt   i[1];
# ifdef VM_INCLUDE_FLOAT
  	tVmFloat f[1];
# endif
} tVmUnion;

// masks and bits of the basic type
#define VM_IMMEDIATE_MASK  ((tVmReference)0x80000000L)
#define VM_TYPE_MASK       ((tVmReference)0xc0000000L)
#define VM_TYPE_HEAP       ((tVmReference)0x80000000L)
#define VM_TYPE_CONST      ((tVmReference)0xc0000000L)

#endif


tHeapId VmNewClass(uint16_t classId);
void VmInitialize(void);
void VmRunMethod(uint16_t mref);
BOOL vm_heap_id_in_use(tHeapId id);

// expand types
void * VmGetAddress(tVmReference ref);

#define nvm_int2stack(x) (~VM_IMMEDIATE_MASK & (x))
tVmInt VmStackToInt(tVmStack val);

#define nvm_ref2stack(x) (x)
#define nvm_stack2ref(x) (x)

#ifdef VM_INCLUDE_FLOAT
tVmStack VmFloatToStack(tVmFloat val);
tVmFloat VmStackToFloat(tVmStack val);
#endif
#define NATIVE_ID(c,m)  ((c<<8)|m)

#define NATIVE_ID2CLASS(m)   (m>>8)
#define NATIVE_ID2METHOD(m)  (m&0xff)
#define NATIVE_ID2FIELD(m)   (m&0xff)

#endif // VM_H
