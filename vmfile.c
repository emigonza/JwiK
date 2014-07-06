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
// vmfile.c
//
// Virtual machine file management API.  The file is stored in flash and this module provides access to and 
// from the file.

#include "vmtypes.h"
//#include "config.h"
#include "vmerror.h"

#include "vmfile.h"
#include "vm.h"
//#include "eeprom.h"
#include "vmfeatures.h"
#include "vmheap.h"
#include "vmstack.h"


static uint8_t vmFile[CODESIZE];
//#include "vmDefaultApp.h"

uint8_t vmFileConstantCount;

// *****************************************************************************************************************8
// Internal Functions
uint16_t CopyCodeToRAM(uint8_t *dst, uint8_t *src, uint16_t len)
{
	uint16_t   i=0;
	for(i=0;i<len;i++)
	{
		*(dst+i) = FlashReadByte(src+i);
		
	}
	return i;
}

// *****************************************************************************************************************8
// Public API

// void VmFileRead(void *dst, void *src, uint16_t len) 
//
// Read 'len' bytes from 'src' to 'dst'
//
// Args:
//  void *dst - pointer to buffer that is the target of the copy
//  void *src - pointer to the source bytes to copy
//  uint16_t len - number of bytes to copy
// Returns: 
//  Nothing
void VmFileRead(void *dst, void *src, uint16_t len) 
{
	src = VMFILLADDR(src);  // remove marker (if present)
	CopyCodeToRAM((uint8_t *)dst, (PGMPTR)src, len);
}
// void VmFileRead08(void *addr)
//
// Read a byte from 'addr'
//
// Args:
//  void *addr - address to read the byte from
// Returns:
//  nothing
uint8_t VmFileRead08(void *addr) 
{
  	uint8_t   val;
  	addr = VMFILLADDR(addr);  // remove marker (if present)
	CopyCodeToRAM((uint8_t*)&val, (PGMPTR)addr, 1);
  	return val;
}
// void VmFileRead16(void *addr)
//
// Read a int from 'addr'
//
// Args:
//  void *addr - address to read the int from
// Returns:
//  nothing
uint16_t VmFileRead16(void *addr) 
{
  	uint16_t   val;
  	addr = VMFILLADDR(addr);  // remove marker (if present)
	CopyCodeToRAM((uint8_t*)&val, (PGMPTR)addr, 2);
  	return val;
}
// void VmFileRead32(void *addr)
//
// Read a dword from 'addr'
//
// Args:
//  void *addr - address to read the dword from
// Returns:
//  nothing
uint32_t VmFileRead32(void *addr) 
{
  	uint32_t   val;
  	addr = VMFILLADDR(addr);  // remove marker (if present)
	CopyCodeToRAM((uint8_t*)&val, (PGMPTR)addr, 4);
  	return val;
}
// void VmWrite08(void *addr)
//
// Write a byte to'addr'
//
// Args:
//  void *addr - address to write byte to
// Returns:
//  nothing
void VmFileWrite08(void *addr, uint8_t dataToWrite) 
{
	// TODO: write VmFileWrite08
}
// void VmFileInitializeWrite(void)
//
// Initialize the write process. This is where you would unlock flash or whatever you
// have to do at the beginning of a file write.
//
// Args:
// Returns:
//  nothing
void VmFileInitializeWrite(void) 
{
	// TODO: write VmFileInitializeWrite
}
// void VmFileFinalizeWrite(void)
//
// Finalize the write process. This is where you would lock flash or whatever you
// have to do at the end of a file write.
//
// Args:
// Returns:
//  nothing
void VmFileFinalizeWrite(void) 
{
	// TODO: write VmFileFinalizeWrite
}

// void VmFileInitialize(void)
//
// Initialize the VM File.
//
// Args:
// Returns:
//  True if everything is OK, false if there was a problem
BOOL VmFileInitialize(void) 
{
	uint16_t   t;
  	uint32_t   features = VmFileRead32(&((tVmFileHeader*)vmFile)->magic_feature);

  	if ((features&NVM_MAGIC_FEAUTURE)!=(features|NVMFILE_MAGIC)) 
	{
    	HandleError(kErrorVmFileBadJuJu);
    	return FALSE;
  	}
  	if(VmFileRead08(&((tVmFileHeader*)vmFile)->version) != NVMFILE_VERSION) 
	{
    	HandleError(kErrorVmFileBadVersion);
    	return FALSE;
  	}

	t = VmFileRead16(&((tVmFileHeader*)vmFile)->string_offset);
	t -= VmFileRead16(&((tVmFileHeader*)vmFile)->constant_offset);
	vmFileConstantCount = t/4;

  	return TRUE;
}
// void VmFileSaveApp(void)
//
// Save a VM application file to flash
//
// Args:
//  u16 index - 
//  uint8_t *buffer - pointer to buffer containing app
//  uint16_t size - number of bytes to copy
// Returns:
//  nothing  
void VmFileSaveApp(uint16_t index, uint8_t *buffer, uint16_t size) 
{
	// TODO: Write VmFileSaveApp
}
// void VmFileGetMethodHeader(uint16_t index)
//
// Gets a method header for the method at 'index'
//
// Args:
//  uint16_t index - method index
// Returns:
//  nothing
tVmFileMethodHeader   *VmFileGetMethodHeader(uint16_t index) 
{
	uint16_t methodOffset;

	
  // get pointer to method header
	tVmFileMethodHeader   *hdrs = ((tVmFileMethodHeader  *)(vmFile+methodOffset)) 
										+ index;
	methodOffset = VmFileRead16(&((tVmFileHeader*)vmFile)->method_offset);
	hdrs = ((tVmFileMethodHeader  *)(vmFile+methodOffset)) 
										+ index;
	return(hdrs);
}
// void VmFileGetMethodHeader(uint8_t index)
//
// Gets a constant for the method at 'index'
//
// Args:
//  uint8_t index - constant index
// Returns:
//  nothing
uint32_t VmFileGetConstant(uint8_t index) 
{
	tVmReference   res;
	uint16_t   addr;
	uint32_t   result;
	if (index<vmFileConstantCount)
	{
		addr = VmFileRead16(&((tVmFileHeader*)vmFile)->constant_offset);
		result = VmFileRead32(vmFile+addr+4*index);
		return result;
	}
	// it's a string!

	res = VM_TYPE_CONST | (index-vmFileConstantCount);
	return res;
}
// void VmFileJumpToMain(void)
//
// Jump to the main method in the application
//
// Args:
//  nothing
// Returns:
//  nothing
void VmFileJumpToMain(void) 
{
	uint8_t   i;
	uint8_t   flags;
	tVmFileHeader   *nvh;
	uint16_t   mn;
	uint16_t   main;
	uint8_t   methodCount=VmFileRead08(&((tVmFileHeader*)vmFile)->methods);
	uint16_t   code_index;
	uint16_t   id;
	uint8_t   args;
	uint8_t   max_locals;
	uint8_t   max_stack;
	tVmFileMethodHeader *mainMethodHeader;
	uint16_t   mainClassId;
	tHeapFrame *heap1;
	tHeapFrame *heap2;
	tHeapId heapId;

	// todo:
	//
	// This is all wrong.  To properly handle the default class, it must exist on the heap
	// so that the stack frame is set up properly and so that there is storage space for local 
	// fields.  This will be a special case because we want to start with an empty stack, whereas
	// new will put the heapId (which is now our reference) on the stack.
	//

	nvh=(tVmFileHeader *)vmFile;
	mn=(nvh)->main;
	main = VmFileRead16(&(nvh)->main);

	mainMethodHeader = (tVmFileMethodHeader *)VmFileGetMethodHeader(main);
	mainClassId = VmFileRead16(&(mainMethodHeader)->id);

	heapId=VmNewClass(mainClassId);

	for(i=0;i<methodCount;i++) 
	{
		tVmFileMethodHeader *mhdr = (tVmFileMethodHeader *)VmFileGetMethodHeader(i);
		code_index = VmFileRead16(&(mhdr)->code_index);
		id = VmFileRead16(&(mhdr)->id);
		flags = VmFileRead08(&(mhdr)->flags) ;
		args = VmFileRead08(&(mhdr)->args);
		max_locals = VmFileRead08(&(mhdr)->max_locals);
		max_stack = VmFileRead08(&(mhdr)->max_stack);
		// is this a clinit method?
		if(flags& FLAG_CLINIT) 
		{
			// Make sure we are running the initialization for the same class that holds the 
			// Main() method.
		  	if( NATIVE_ID2CLASS(mainClassId) == NATIVE_ID2CLASS(id) )
		  		VmRunMethod(i);
		}
	}
	
	heap1 = HeapSearch(1);
	heap2 = HeapSearch(2);
	
	StackPush(heapId);
	// determine method description address and code
	VmRunMethod(main);
}
// void VmFileGetAddress(uint16_t ref)
//
// Gets an address for a string at 'index'
//
// Args:
//  uint16_t ref - method index
// Returns:
//  nothing
void *VmFileGetAddress(uint16_t ref) 
{
  // get pointer to string
  	uint16_t   *refs = (uint16_t  *)(vmFile + VmFileRead16(&((tVmFileHeader*)vmFile)->string_offset));

	return((uint8_t*)refs + VmFileRead16(refs+ref));
}
// void VmFileGetClassFieldCount(uint8_t index)
//
// Gets a count of the fields in a class for the class at 'index'
//
// Args:
//  uint8_t index - class index
// Returns:
//  nothing
uint8_t VmFileGetClassFieldCount(uint8_t index) 
{
	uint8_t retVal;
	tVmFileHeader *vmfh;
	void *addr;
	tVmFileClassHeader *ch;

	// fixed the pointer math.  
	vmfh = (tVmFileHeader*)vmFile;
	ch = &vmfh->class_hdr;
	addr = &(ch[index].fields);
	retVal = VmFileRead08(addr);
  	return retVal;
}
// uint8_t VmFileGetStaticFieldCount(void)
//
// Gets a method header for the method at 'index'
//
// Args:
//  Nothing
// Returns:
//  Count of static fields in application
uint8_t VmFileGetStaticFieldCount(void) 
{
  	return VmFileRead08(&((tVmFileHeader*)vmFile)->static_fields);
}

#ifdef VM_INCLUDE_INHERITANCE
// uint8_t VmFileGetMethodByFixedClassAndId(uint8_t classId, uint8_t id)
//
// Gets a method by class id and method id
//
// Args:
//  uint8_t classId - class id
//  uint8_t id - method id
// Returns:
//  uint8_t containing method number
uint8_t VmFileGetMethodByFixedClassAndId(uint8_t classId, uint8_t id) 
{
  	uint8_t   i;
  	tVmFileMethodHeader   mhdr, *mhdr_ptr;


	for(i=0;i<VmFileRead08(&((tVmFileHeader*)vmFile)->methods);i++) 
	{
	    // load new method header into ram
	    mhdr_ptr = VmFileGetMethodHeader(i);
	    VmFileRead(&mhdr, mhdr_ptr, sizeof(tVmFileMethodHeader));

	    if(((mhdr.id >> 8) == classId) && ((mhdr.id & 0xff) == id)) 
		{
			// we found our guy, so return it.
	    	return i;
    	}
  	}
	// return 0xff if the classId and id are not found.
	return 0xff;
}
// uint8_t VmFileGetMethodByClassAndId(uint8_t classId, uint8_t id)
//
// Gets a method by class id and method id
//
// Args:
//  uint8_t classId - class id
//  uint8_t id - method id
// Returns:
//  uint8_t containing method number

// TODO: Rewrite this.  It will loop endlessly if the result is not found.  THIS IS A VERY BAD DESIGN
uint8_t VmFileGetMethodByClassAndId(uint8_t classId, uint8_t id) 
{
	uint8_t   mref;

	for(;;) 
	{
		if((mref = VmFileGetMethodByFixedClassAndId(classId, id)) != 0xff)
		  	return mref;
		classId = VmFileRead08(&((tVmFileHeader*)vmFile)->class_hdr[classId].super);
	}

	return 0;
}
#endif
