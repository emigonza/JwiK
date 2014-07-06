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
// config.h
//
// Configuration file.  Contains config definitions that determine the platform and features to be compiled

#ifndef CONFIG_H
#define CONFIG_H

// *** Platform Configuration ***
//
// Processor:
// #define R5F100EEANA
#define JWIK


// Processor clock frequency
#define CPUCLOCK 16000000UL

#define CODESIZE 4096
#define HEAPSIZE (4096-256)    // uVM itself requires 256 Bytes RAM


// vm setup

#define VM_INCLUDE_ARRAY            // enable arrays
#define VM_INCLUDE_SWITCH           // support switch instruction
#define VM_INCLUDE_INHERITANCE      // support for inheritance
#define VM_INCLUDE_FLASH_PROGRAM

// native setup
//#define VM_INCLUDE_STDIO            // enable native stdio support adds 2K
//#define VM_INCLUDE_MATH				// adds 5K
//#define VM_INCLUDE_FORMATTER		// adds 4K

// marker used to indicate, that this item is stored in eeprom
#define NVMFILE_FLAG     0x8000

//#define VM_INCLUDE_FLOAT			// adds 3K
#define VM_INCLUDE_32BIT_WORD

#endif // CONFIG_H
