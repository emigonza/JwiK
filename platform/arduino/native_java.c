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
// TODO: Rename this file
// native-impl.c
//
// Public API for native class and method implementations.

#include "platform_types.h"
#include "config.h"
#include "vmerror.h"

#include "vm.h"
#include "vmfile.h"
#include "native_java.h"

#include "vmstack.h"

#ifdef VM_INCLUDE_STDIO
#include "native_stdio.h"
#endif

#ifdef VM_INCLUDE_MATH
#include "native_math.h"
#endif

#ifdef VM_INCLUDE_FORMATTER
#include "native_formatter.h"
#endif
#include "microapi.h"
#include "native_jwik.h"
#include "uart.h"

#define MAXTIMERS 4
uint16_t timers[MAXTIMERS];
// called by microcontroller every 1mSec.  Timeouts should be tied to this
void Handle1MsInterrupt()
{
	int i;
	for(i=0;i<MAXTIMERS;i++)
		timers[i]++;
	
}

// ****************************************************************************************************
// Internal Functions

// Invokes a method on the java.lang.object.  The only method supported is <init>.  Any other method
// will generate an error.
void InvokeJavaObjectMethod(uint8_t methodReference) 
{
  if(methodReference == NATIVE_METHOD_INIT) 
  {
    /* ignore object constructor ... */
    StackPop();  // pop object reference
  } 
  else 
    HandleError(kErrorNativeUnknownMethod);
}


// ****************************************************************************************************
// Public API

void WaitMsec(int msec)
{
	timers[0]=0;
	while(timers[0]<msec);
	return;
}

// tHeapId CreateNewNativeObject(uint16_t methodReference)
//
// Creates a new native object on the heap and returns the heapId
//
// Args:
//  uint16_t classReference - reference of native class to instantiate
// Returns
//  tHeapId - id of object on heap
tHeapId CreateNewNativeObject(uint16_t methodReference) 
{
	tHeapId heapId;
	switch(NATIVE_ID2CLASS(methodReference))
	{
		case NATIVE_CLASS_STRINGBUFFER:
			heapId = HeapAlloc(FALSE, 1);
		    StackPush(VM_TYPE_HEAP | heapId);
			break;
		default:
			HandleError(kErrorNativeUnknownClass);
			break;

	}
	return heapId;
	/* TODO: Remove this once the switch statement is verified

  if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_STRINGBUFFER) {
    // create empty stringbuf object and push reference onto stack
    StackPush(VM_TYPE_HEAP | HeapAlloc(FALSE, 1));
  } else 
    HandleError(kErrorNativeUnknownClass);*/
}

// void InvokeNativeMethod(uint16_t methodReference)
//
// Invokes a native method of a native class.  Anytime a new class is defined with native methods,
// those methods need to be included in the enum in native.h and the switch statement in this function.
//
// Arguments:
//    uint16_t methodReference - contains the class number and the method
// Returns:
//   void - nothing
void InvokeNativeMethod(uint16_t methodReference) 
{
	uint8_t clss = (uint8_t)( (methodReference>>8) & 0xFF);
	uint8_t method = (uint8_t) (methodReference & 0x0FF);
	switch(NATIVE_ID2CLASS(methodReference))
	{
		case NATIVE_CLASS_OBJECT:
			InvokeJavaObjectMethod(NATIVE_ID2METHOD(methodReference));
			break;
#ifdef VM_INCLUDE_STDIO
		case NATIVE_CLASS_PRINTSTREAM:
			NativeStdIOPrintStreamInvoke(NATIVE_ID2METHOD(methodReference));
			break;

		case NATIVE_CLASS_STRINGBUFFER:
			NativeJavaStringBufferInvoke(NATIVE_ID2METHOD(methodReference));
			break;
		case NATIVE_CLASS_INPUTSTREAM:
			NativeStdIOInputStreamInvoke(NATIVE_ID2METHOD(methodReference));
			break;
#endif
#ifdef VM_INCLUDE_MATH
		case NATIVE_CLASS_MATH:
			NativeMathInvoke(NATIVE_ID2METHOD(methodReference));
			break;
#endif
#ifdef VM_INCLUDE_FORMATTER
		case NATIVE_CLASS_FORMATTER:
			NativeFormatInvoke(NATIVE_ID2METHOD(methodReference));
			break;
#endif
		//case NATIVE_CLASS_RFM50:
		//	NativeRFM50Invoke(NATIVE_ID2METHOD(methodReference));
		//	break;
		case NATIVE_CLASS_BRIKPINS:
			NativeBrikInvoke(NATIVE_ID2METHOD(methodReference));
			break;
		default:
			HandleError(kErrorNativeUnknownClass);
			break;
			
	}
	/* TODO: Delete this once the switch statement is checked out.
  // check for native classes/methods
  if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_OBJECT) {
    InvokeJavaObjectMethod(NATIVE_ID2METHOD(methodReference));

#ifdef VM_INCLUDE_STDIO
  } else if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_PRINTSTREAM) {
    NativeStdIOPrintStreamInvoke(NATIVE_ID2METHOD(methodReference));
  } else if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_INPUTSTREAM) {
    NativeStdIOInputStreamInvoke(NATIVE_ID2METHOD(methodReference));
  } else if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_STRINGBUFFER) {
    NativeJavaStringBufferInvoke(NATIVE_ID2METHOD(methodReference));
#endif

#ifdef VM_INCLUDE_MATH
    // the math class
  } else if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_MATH) {
    NativeMathInvoke(NATIVE_ID2METHOD(methodReference));
#endif

#ifdef VM_INCLUDE_FORMATTER
    // the formatter class
  } else if(NATIVE_ID2CLASS(methodReference) == NATIVE_CLASS_FORMATTER) {
    NativeFormatInvoke(NATIVE_ID2METHOD(methodReference));
#endif


  } else
    HandleError(kErrorNativeUnknownClass);
	*/
}

// void NativeInitialize(void)
// 
// Perform and native library initialization here.  For example, if an AD library is added, we would call it initialization 
// function here.
//
// Arguments:
//   None
// Returns:
//   Nothing
void NativeInitialize()
{
	InitializeMicroAPI();
	InitializeUart(b9600);
	//EnableInterrupts;

}

