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
// native_stdio.c
//
// Public API for native stdio class and its methods



#include "platform_types.h"
#include "uart.h"
#include "config.h"
#include "vmerror.h"
#ifdef VM_INCLUDE_STDIO
#include "utils.h"
#include "vm.h"
#include "native_java.h"
#include "native_stdio.h"
#include "vmstack.h"
#include "native_string.h"


// **********************************************************************************************************
// Internal Functions

// convert an integer to a string
void NativeStdIOitoa(char *str, tVmInt val) 
{
  	tVmInt   m;
#ifdef VM_INCLUDE_16BIT_WORD
  	tVmInt   div = 10000;  // max num to be output with 15 uint8_t integer 
#else  
  	tVmInt   div = 1000000000L;  // max num to be output 
#endif
  	BOOL   printed = FALSE;
  
  	if(val < 0) 
	{
    	*str++ = '-';
    	val = -val;
	}
  
  	while(div > 0) 
	{
    	m = val / div;
    	if(m) printed = TRUE;
    
    	if((printed)||(div == 1)) 
      		*str++ = '0' + m;
    
    	val = val % div;
    	div /= 10;
  	}
  
  	*str++ = 0;
}

#ifdef VM_INCLUDE_FLOAT
// convert a float to a string
void NativeStdIOftoa(char   *str, tVmFloat val) 
{
	char   * sep;
	tVmInt   i;
  	if (val<0)
  	{
    	*str++='-';
    	val=-val;
  	}

	i = val;
	NativeStdIOitoa(str, i);
	while (*str) str++; // to string end
	sep = str;
	val+=1-i;
	i=val*1000000;
	NativeStdIOitoa(str, i);
	sep[0] ='.';
	while (*str) str++; // to string end
	while (*(--str)=='0') *str=0;
	if (*str=='.') *str=0;
}
#endif

// send a string to the console and append return if 'ret' is true
static void NativeStdPrint(char *str, BOOL ret) 
{
  	uint8_t   chr;

	// If the string is in th VM File, send it from there, otherwise
	// send it from RAM.
	if(VMISFILEFLAGSET(str)) 
	   	while((chr = VmFileRead08(str++)))
      		UartSendByte(chr);
  	else
    	while(*str)
      		UartSendByte(*str++);

  	if(ret)
    	UartSendByte('\n');
}

// **********************************************************************************************************
// Public API

// void NativeStdIOPrintStreamInvoke(uint8_t methodReference)
//
// invoke a native method within class java/io/PrintStream
//
// Args:
//  uint8_t methodReference - reference of method to invoke
// Returns:
//  Nothing
void NativeStdIOPrintStreamInvoke(uint8_t methodReference) 
{
  	char   tmp[8];

	switch(methodReference)
	{
		case NATIVE_METHOD_PRINTLN_STR:
			NativeStdPrint(StackPopAddr(), TRUE);
			break;
		case NATIVE_METHOD_PRINTLN_INT:
			NativeStdIOitoa((char*)tmp, StackPopInt());
		    NativeStdPrint(tmp, TRUE);
			break;
		case NATIVE_METHOD_PRINT_STR:
			NativeStdPrint(StackPopAddr(), FALSE);
			break;
		case NATIVE_METHOD_PRINT_INT:
			NativeStdIOitoa((char*)tmp, StackPopInt());
		    NativeStdPrint(tmp, FALSE);
			break;
		case NATIVE_METHOD_PRINTLN_CHAR:
			UartSendByte(StackPopInt());
    		UartSendByte('\n');
			break;	
		case NATIVE_METHOD_PRINT_CHAR:
			UartSendByte(StackPopInt());
			break;
#ifdef VM_INCLUDE_EXT_STDIO
  		case NATIVE_METHOD_FORMAT:
		    NativeStdPrint(StackPopAddr(), FALSE);
    		StackPopInt(); 
    		StackPush(StackPeek(0)); 
#endif   
		default:
			HandleError(kErrorNativeUnknownMethod);
			break;
	}
	// TODO: delete this code once the new switch statement is verified
/*
  if(methodReference == NATIVE_METHOD_PRINTLN_STR) {
    NativeStdPrint(StackPeekInt(), TRUE);
  } else if(methodReference == NATIVE_METHOD_PRINTLN_INT) {
    NativeStdIOitoa((char*)tmp, StackPopInt());
    NativeStdPrint(tmp, TRUE);
  } else if(methodReference == NATIVE_METHOD_PRINT_STR) {
    NativeStdPrint(StackPeekInt(), FALSE);
  } else if(methodReference == NATIVE_METHOD_PRINT_INT) {
    NativeStdIOitoa((char*)tmp, StackPopInt());
    NativeStdPrint(tmp, FALSE);
  } else if(methodReference == NATIVE_METHOD_PRINTLN_CHAR) {
    UartSendByte(StackPopInt());
    UartSendByte('\n');
  } else if(methodReference == NATIVE_METHOD_PRINT_CHAR) {
    UartSendByte(StackPopInt());
#ifdef VM_INCLUDE_EXT_STDIO
  } else if(methodReference == NATIVE_METHOD_FORMAT) {
    NativeStdPrint(StackPeekInt(), FALSE);
    StackPopInt(); // TODO
    StackPush(StackPeek(0)); // duplicate this ref
#endif    
  } else
    HandleError(kErrorNativeUnknownMethod);

  // popping the reference at the end only works as long
  // as none of the native methods places anything on the stack
  
  StackPop(); // pop output stream reference
  */
}

// void NativeStdIOInputStreamInvoke(uint8_t methodReference)
//
// invoke a native method within class java/io/InputStream
// 
// Args:
//  uint8_t methodReference - reference to method to be invoked
// Returns:
//  nothing
void NativeStdIOInputStreamInvoke(uint8_t methodReference) 
{

	// popping the reference at the beginning only works as long
	// as all of the native methods place something on the stack

	StackPop(); // pop input stream reference
	
	switch(methodReference)
	{
		case NATIVE_METHOD_INPUTSTREAM_AVAILABLE:
			StackPush(UartBytesAvailable());
			break;
		case NATIVE_METHOD_INPUTSTREAM_READ:
			StackPush(UartReadByte());
			break;
		default:
			HandleError(kErrorNativeUnknownMethod);
			break;
	}
	// TODO: delete this code once the new switch statement is verified
/*
	if(methodReference == NATIVE_METHOD_INPUTSTREAM_AVAILABLE) {
	StackPush(UartBytesAvailable());
	} else if(methodReference == NATIVE_METHOD_INPUTSTREAM_READ) {
	StackPush(UartReadByte());
	} else 
	HandleError(kErrorNativeUnknownMethod);
	*/
}    

// void NativeJavaStringBufferInvoke(uint8_t methodReference)
//
// invoke a native method within class java/lang/StringBuffer
//
// Args:
//  uint8_t methodReference - reference of method to invoke
// Returns:
//  nothing
void NativeJavaStringBufferInvoke(uint8_t methodReference) 
{
	char   *src, *src0, *src1, *dst;
	uint16_t   len;
#ifdef VM_INCLUDE_FLOAT
    char   tmp[15];
#else
# ifdef VM_INCLUDE_32BIT_WORD
    char   tmp[10];
# else
    char   tmp[5];
# endif
#endif
	tHeapId   id;

	switch(methodReference)
	{
		case NATIVE_METHOD_INIT:
			*(char*)StackPopAddr() = 0;  // should this be StackPeekInt()
			break;
		case NATIVE_METHOD_INIT_STR:
			// get the address of the string on the stack
    		src = StackPeekAddr(0);
			// check source of string
			len = NativeStrLen(src);
			// resize existing object
			HeapRealloc(StackPeek(1) & ~VM_TYPE_MASK, len + 1);
			// and copy string to new object
			src = StackPeekAddr(0);
			dst = HeapGetAddress(StackPeek(1) & ~VM_TYPE_MASK);
			NativeStrCpy(dst, src);
			// get rid of source references still on the stack
			StackPop(); 
			StackPop(); 
			break;
		case NATIVE_METHOD_APPEND_STR:
		case NATIVE_METHOD_APPEND_INT:
		case NATIVE_METHOD_APPEND_CHR:
		case NATIVE_METHOD_APPEND_FLOAT:
		    if(methodReference == NATIVE_METHOD_APPEND_STR) 
			{
		      	// appending a string is simple
		      	src1 = StackPeekAddr(0);
		      	// check source of string
		      	len = NativeStrLen(src1);
    		} 
			else 
			{
      			if(methodReference == NATIVE_METHOD_APPEND_INT) 
				{
        			// integer has to be converted
       		 		NativeStdIOitoa(tmp, StackPeekInt(0));

      			} 
#ifdef VM_INCLUDE_FLOAT
				else if(methodReference == NATIVE_METHOD_APPEND_FLOAT) 
				{
       				// integer has to be converted
        			NativeStdIOftoa(tmp, StackPeekFloat(0));
      			} 
#endif
				else 
				{
					// character is directly appended
					tmp[0] = StackPeek(0);
					tmp[1] = 0;
      			}

      			src1 = tmp;
      			len = UtilsGetStringLength((char*)src1);
    		}

			// this is always a string
			src0 = StackPeekAddr(1);
			len += NativeStrLen(src0);

			// create a new object 
			id = HeapAlloc(FALSE, len + 1);

			// alloc may have had an impact on heap, so get address again
			src0 = StackPeekAddr(1);
			if(methodReference == NATIVE_METHOD_APPEND_STR) 
				src1 = StackPeekAddr(0);

			// handle nvmfile memory and ram
			dst = HeapGetAddress(id);
			NativeStrCpy(dst, src0);
			NativeStrCat(dst, src1);

			// get rid of source references still on the stack
			StackPop(); StackPop();  // 
			// place new reference on the stack
			StackPush(VM_TYPE_HEAP | id);
		case NATIVE_METHOD_TOSTRING:
			// do nothing since this is a string
			break;
		default:
			HandleError(kErrorNativeUnknownMethod);
			break;
			
	}
	// TODO: delete this code once the switch statement is tested.
/*
  if(methodReference == NATIVE_METHOD_INIT) {
    // make this an empty string
    *(char*)StackPeekInt() = 0;
  } else if(methodReference == NATIVE_METHOD_INIT_STR) {
    char   *src, *dst;
    uint16_t   len;

    src = StackPeekAddr(0);
    // check source of string
    len = NativeStrLen(src);

    // resize existing object
    HeapRealloc(StackPeek(1) & ~VM_TYPE_MASK, len + 1);

    // and copy string to new object
    src = StackPeekAddr(0);
    dst = HeapGetAddress(StackPeek(1) & ~VM_TYPE_MASK);
    NativeStrCpy(dst, src);

    // get rid of source references still on the stack
    StackPop(); StackPop(); 

  } else if((methodReference == NATIVE_METHOD_APPEND_STR)||
	    (methodReference == NATIVE_METHOD_APPEND_INT)||
	    (methodReference == NATIVE_METHOD_APPEND_CHR)||
            (methodReference == NATIVE_METHOD_APPEND_FLOAT)) 
	{
			    char   *src0, *src1, *dst;
			#ifdef VM_INCLUDE_FLOAT
			    char   tmp[15];
			#else
			# ifdef VM_INCLUDE_32BIT_WORD
			    char   tmp[10];
			# else
			    char   tmp[5];
			# endif
			#endif
			    uint16_t   len;
				tHeapId   id;
    
    	if(methodReference == NATIVE_METHOD_APPEND_STR) 
		{
		      // appending a string is simple
		      src1 = StackPeekAddr(0);
		      // check source of string
		      len = NativeStrLen(src1);
    	} 
		else 
		{
      		if(methodReference == NATIVE_METHOD_APPEND_INT) 
			{
        		// integer has to be converted
       		 NativeStdIOitoa(tmp, StackPeekInt(0));
#ifdef VM_INCLUDE_FLOAT
      		} 
			else if(methodReference == NATIVE_METHOD_APPEND_FLOAT) 
			{
       			// integer has to be converted
        		NativeStdIOftoa(tmp, StackPeekFloat(0));
#endif
      		} 
			else 
			{
				// character is directly appended
				tmp[0] = StackPeek(0);
				tmp[1] = 0;
      		}

      		src1 = tmp;
      		len = UtilsGetStringLength((char*)src1);
    	}

		// this is always a string
		src0 = StackPeekAddr(1);
		len += NativeStrLen(src0);

		// create a new object 
		id = HeapAlloc(FALSE, len + 1);

		// alloc may have had an impact on heap, so get address again
		src0 = StackPeekAddr(1);
		if(methodReference == NATIVE_METHOD_APPEND_STR) 
		  src1 = StackPeekAddr(0);

		// handle nvmfile memory and ram
		dst = HeapGetAddress(id);
		NativeStrCpy(dst, src0);
		NativeStrCat(dst, src1);

		// get rid of source references still on the stack
		StackPop(); StackPop();  // 
		// place new reference on the stack
		StackPush(VM_TYPE_HEAP | id);

	} 
	else if(methodReference == NATIVE_METHOD_TOSTRING) 
	{
    	// toString does nothing, since the object already is
   		// a valid string
  	} 
	else 
	{
    	//DEBUGF("unknown method in java/lang/StringBuffer\n");
    	HandleError(kErrorNativeUnknownMethod);
  	}
	*/
}
#endif

