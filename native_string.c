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
//  string.c
//
// Virtual machine string management
//


//#include "config.h"
#include "vmerror.h"

#ifdef VM_INCLUDE_STDIO

#include "vm.h"
#include "utils.h"
#include "vmstack.h"
#include "native_string.h"



// int8_t NativeGetChar(int8_t* src)
//
// Get character from the string position pointed to by 'src'
//
// Args:
//  int8_t* src - string position
// Returns:
//  int8_t character from string
int8_t NativeGetChar(int8_t* src) 
{
  	// check if string resides in nvm file memory (e.g. eeprom)
  	if(VMISFILEFLAGSET(src))
    	return VmFileRead08(src);
  	else
    	return *src;
}
// void NativeStrPartialCpy(int8_t *dst, int8_t* src, int16_t count)
//
// Copy a string to RAM
//
// Args:
//  int8_t* dst - ram buffer to copy to
//  int8_t* src - string position
//  int16_t count - number of characters to copy
// Returns:
//  Nothing
void NativeStrPartialCpy(int8_t *dst, int8_t* src, int16_t count) 
{

  	// check if string resides in nvm file memory (e.g. eeprom)
  	if(VMISFILEFLAGSET(src))
    	while(count--&&(*dst++ = VmFileRead08(src++)));
  	else 
    	while(count--&&(*dst++ = *src++));
}
// void NativeStrCpy(int8_t *dst, int8_t* src, int16_t count)
//
// Copy a string to RAM
//
// Args:
//  int8_t* dst - ram buffer to copy to
//  int8_t* src - string position
//  int16_t count - number of characters to copy
// Returns:
//  Nothing
void NativeStrCpy(char *dst, char* src) 
{

  	// check if string resides in nvm file memory (e.g. eeprom)
  	if(VMISFILEFLAGSET(src))
    	while((*dst++ = VmFileRead08(src++)));
  	else 
    	while((*dst++ = *src++));
}

// uint16_t NativeStrLen(int8_t *str)
//
// Get string length
//
// Args:
//  int8_t* str- pointer to string
// Returns:
//  Nothing
uint16_t NativeStrLen(int8_t *str) 
{
  	uint16_t   len=0;

  	// check if string resides in nvm file memory (e.g. eeprom)
  	if(VMISFILEFLAGSET(str))
    	while(VmFileRead08(str++)) 
			len++;
  	else
    	while(*str++) len++;
  
  	return len;
}
// void NativeStrCat(int8_t *dst, int8_t* src, int16_t count)
//
// Concatonate two strings
//
// Args:
//  int8_t* dst - ram buffer to copy to
//  int8_t* src - string position
//  int16_t count - number of characters to copy
// Returns:
//  Nothing

void NativeStrCat(int8_t *dst, int8_t *src) 
{
  	while(*dst) dst++;         // skip string
  	NativeStrCpy(dst, src);   // attach it
}

#endif // VM_INCLUDE_STDIO
