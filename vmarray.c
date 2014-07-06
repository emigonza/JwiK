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
// array.c
//
// virtual machine array management

//#include "vmtypes.h"
//#include "config.h"
#include "vmerror.h"

#include "vm.h"
#include "vmarray.h"
#include "vmheap.h"

#ifdef VM_INCLUDE_ARRAY
// ***************************************************************************************************************************
// Internal functions

uint8_t ArrayTypeLen(ArrayElementTypes elementType) 
{
	uint8_t   len = 0;
	switch(elementType)
	{ 
		case kBoolean:
		case kChar:
		case kByte:
			len = sizeof(tVmByte);
			break;
		case kShort:
			len = sizeof(tVmShort);
			break;
		case kInt:
			len = sizeof(tVmInt);
			break;
		default:
			HandleError(kErrorArrayIllegalType);
	}
	return len;
	// TODO: delete this code once the new switch statement is tested
	/*
  if((type == T_BOOLEAN)||(type == T_CHAR)||(type == T_BYTE))
    return sizeof(tVmByte);
  
  if(type == T_SHORT)
    return sizeof(tVmShort);
  if(type == T_INT)
    return sizeof(tVmInt);

  HandleError(kErrorArrayIllegalType);
  return 0;  // to make compiler happy
  */
}

// ****************************************************************************************************************************
// Public API
// ****************************************************************************************************************************

// tHeapId ArrayNew(tVmInt length, uint8_t type)
//
// Create a new array.
//
// Args:
//  tVmInt length - number of elements in array
//  uint8_t type - type of array elements
// Results:
//  tHeapId - id of the array
tHeapId ArrayNew(tVmInt length, uint8_t type) 
{
	tHeapId   id;

  	id = HeapAlloc(FALSE, 1 + length * ArrayTypeLen(type));

  // store type in first byte
  	*(uint8_t*)(HeapGetAddress(id)) = type;

  	return id;
}

// tVmInt ArrayLen(tHeapId id)
//
// Gets the length of an array by its ID
//
// Args:
//  tHeapId id - id of array
// Returns:
//  tVmInt - length of the array as an integer
tVmInt ArrayLen(tHeapId id) 
{

  	return(HeapGetLength(id)/
		ArrayTypeLen(*(uint8_t*)HeapGetAddress(id)));
}

// void ArraySetByteValue(tHeapId id, tVmInt index, tVmByte value)
//
// Store a value in the array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of target element
//  tVmByte value - value to store
// Returns:
//  nothing 
void ArraySetByteValue(tHeapId id, tVmInt index, tVmByte value) 
{
	tVmByte   * ptr = (tVmByte   *)HeapGetAddress(id) + 1;
	ptr[index] = value;
}

// void ArrayGetByteValue(tHeapId id, tVmInt index)
//
// Gets a byte from an array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of value to get
// Returns:
//  tVmByte value of array[index]
tVmByte ArrayGetByteValue(tHeapId id, tVmInt index) 
{
	tVmByte   * ptr = (tVmByte   *)HeapGetAddress(id) + 1;
	return ptr[index];
}

// void ArraySetIntValue(tHeapId id, tVmInt index, tVmInt value)
//
// Store a value in the array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of target element
//  tVmInt value - value to store
// Returns:
//  nothing 
void ArraySetIntValue(tHeapId id, tVmInt index, tVmInt value) 
{
	tVmInt   * ptr = (tVmInt   *)((uint8_t*)HeapGetAddress(id) + 1);
	ptr[index] = value;
	HeapCheck();
}
// void ArrayGetIntValue(tHeapId id, tVmInt index)
//
// Gets a int from an array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of value to get
// Returns:
//  tVmInt value of array[index]
tVmInt ArrayGetIntValue(tHeapId id, tVmInt index) 
{
	tVmInt   * ptr = (tVmInt   *)((uint8_t*)HeapGetAddress(id) + 1);
	return ptr[index];
}

#ifdef VM_INCLUDE_FLOAT
// void ArraySetFloatValue(tHeapId id, tVmInt index, tVmFloat value)
//
// Store a value in the array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of target element
//  tVmFloat value - value to store
// Returns:
//  nothing 
void ArraySetFloatValue(tHeapId id, tVmInt index, tVmFloat value) 
{
	tVmFloat   * ptr = (tVmFloat  *)((uint8_t*)HeapGetAddress(id) + 1);
	ptr[index] = value;
	HeapCheck();
}
// void ArrayGetFloatValue(tHeapId id, tVmInt index)
//
// Gets a Float from an array
//
// Args:
//  tHeapId id - id of array
//  tVmInt index - index of value to get
// Returns:
//  tVmFloat value of array[index]
tVmFloat ArrayGetFloatValue(tHeapId id, tVmInt index) 
{
	tVmFloat   * ptr = (tVmFloat  *)((uint8_t*)HeapGetAddress(id) + 1);
	return ptr[index];
}
#endif

#endif
