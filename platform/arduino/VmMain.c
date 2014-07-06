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
// VmMain.c
//
// Initializes the Si1000 RFIC hardware platform, native library, and virtual machine and then begins
// execution at main.
//#define SDCC
//#define __C51__
#include "platform_types.h"
#include "config.h"

#include "uart.h"
#include "vmfile.h"
#include "vm.h"

#include "native_java.h"
#include "platform.h"


int main() 
{
	// Initialize the native libraries
	NativeInitialize();
	// Initialize the hardware platform
	PlatformInitialize();
	// The vm class file is stored in NV Memory.  Initialize it now.
	VmFileInitialize();
	// Initialize the vm
	VmInitialize();
	// Everything is ready to go, so jump to main and start executing
	VmFileJumpToMain();

	return 0;
}
