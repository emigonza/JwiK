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
// vmfeatures.h
//
// This file contains all of the defines that control which features are compiled into the VM

#ifndef _NVMFEAUTURES_H_
#define _NVMFEAUTURES_H_

// checking flags...
#ifdef VM_INCLUDE_SWITCH

#ifndef VM_INCLUDE_LOOKUPSWITCH
#define VM_INCLUDE_LOOKUPSWITCH
#endif

#ifndef VM_INCLUDE_TABLESWITCH
#define VM_INCLUDE_TABLESWITCH
#endif

#endif

// checking int size flags
#ifdef VM_INCLUDE_16BIT_WORD
#ifdef VM_INCLUDE_32BIT_WORD
#error "you must define either VM_INCLUDE_16BIT_WORD or VM_INCLUDE_32BIT_WORD, not both!"
#endif
#else 
#ifndef VM_INCLUDE_32BIT_WORD
#define VM_INCLUDE_16BIT_WORD
#endif
#endif

// checking float flags
#ifdef VM_INCLUDE_FLOAT
#ifndef VM_INCLUDE_32BIT_WORD
#error "VM_INCLUDE_FLOAT is only allowed with VM_INCLUDE_32BIT_WORD!"
#endif
#endif


#define NVMFILE_VERSION    2
#define NVMFILE_MAGIC      0xBE000000L


#define NVM_FEAUTURE_LOOKUPSWITCH (1L<<0)
#define NVM_FEAUTURE_TABLESWITCH  (1L<<1)
#define NVM_FEAUTURE_32BIT        (1L<<2)
#define NVM_FEAUTURE_FLOAT        (1L<<3)
#define NVM_FEAUTURE_ARRAY        (1L<<4)
#define NVM_FEAUTURE_INHERITANCE  (1L<<5)
#define NVM_FEAUTURE_EXTSTACK     (1L<<6)

#ifndef VM_INCLUDE_LOOKUPSWITCH
#undef NVM_FEAUTURE_LOOKUPSWITCH
#define NVM_FEAUTURE_LOOKUPSWITCH 0
#endif

#ifndef VM_INCLUDE_TABLESWITCH
#undef NVM_FEAUTURE_TABLESWITCH
#define NVM_FEAUTURE_TABLESWITCH 0
#endif

#ifndef VM_INCLUDE_32BIT_WORD
#undef NVM_FEAUTURE_32BIT
#define NVM_FEAUTURE_32BIT 0
#endif

#ifndef VM_INCLUDE_FLOAT
#undef NVM_FEAUTURE_FLOAT
#define NVM_FEAUTURE_FLOAT 0
#endif

#ifndef VM_INCLUDE_ARRAY
#undef NVM_FEAUTURE_ARRAY
#define NVM_FEAUTURE_ARRAY 0
#endif

#ifndef VM_INCLUDE_INHERITANCE
#undef NVM_FEAUTURE_INHERITANCE
#define NVM_FEAUTURE_INHERITANCE 0
#endif

#ifndef VM_INCLUDE_EXTSTACKOPS
#undef NVM_FEAUTURE_EXTSTACK
#define NVM_FEAUTURE_EXTSTACK 0
#endif


#define NVM_MAGIC_FEAUTURE (NVMFILE_MAGIC\
                           |NVM_FEAUTURE_LOOKUPSWITCH\
                           |NVM_FEAUTURE_TABLESWITCH\
                           |NVM_FEAUTURE_32BIT\
                           |NVM_FEAUTURE_FLOAT\
                           |NVM_FEAUTURE_ARRAY\
                           |NVM_FEAUTURE_INHERITANCE)


#endif // _NVMFEAUTURES_H_
