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
// vmfile.h


#ifndef NVMFILE_H
#define NVMFILE_H

#include "vmtypes.h"
#include "vm.h"

typedef struct 
{
	uint8_t super;
	uint8_t fields;
} tVmFileClassHeader;

typedef struct 
{
	uint16_t code_index;
	uint16_t id;           // class and method id
	uint8_t flags;
	uint8_t args;
	uint8_t max_locals;
	uint8_t max_stack;
} tVmFileMethodHeader;

typedef struct 
{
	uint32_t magic_feature;    // old 32 uint8_t magic is replaced by 8 uint8_t magic and 24 feauture bits
	uint8_t version;
	uint8_t methods;          // number of methods in this file
	uint16_t main;             // index of main method
	uint16_t constant_offset;
	uint16_t string_offset;
	uint16_t method_offset;
	uint8_t static_fields;
	tVmFileClassHeader* class_hdr;
} tVmFileHeader;

// marker that indicates, that a method is a classes init method
#define FLAG_CLINIT 1

extern uint8_t vmFileConstantCount;

void   VmFileSaveApp(uint16_t index, uint8_t *buffer, uint16_t size);

BOOL VmFileInitialize(void);
void   VmFileJumpToMain(void);
void   VmFileWriteByte(void *addr, uint8_t writedata);

void   *VmFileGetAddress(uint16_t ref);
uint8_t  VmFileGetClassFieldCount(uint8_t index);
uint8_t  VmFileGetStaticFieldCount(void);
uint32_t  VmFileGetConstant(uint8_t index);

void   VmFileRead(void *dst, void *src, uint16_t len);
uint8_t  VmFileRead08(void *addr);
uint16_t  VmFileRead16(void *addr);
uint32_t  VmFileRead32(void *addr);
uint8_t  VmFileGetMethodByClassAndId(uint8_t class, uint8_t id);

tVmFileMethodHeader   *VmFileGetMethodHeader(uint16_t index);

#define VMSETFILEFLAG(a)     (void*)(((PTR)a) | NVMFILE_FLAG)
#define VMISFILEFLAGSET(a)   (((PTR)a) & NVMFILE_FLAG)
#define VMFILLADDR(a)        (void*)(((PTR)a) & ~NVMFILE_FLAG)

#endif // NVMFILE_H
