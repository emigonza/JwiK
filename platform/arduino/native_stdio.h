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
// native_stdio.h
//

#ifndef NATIVE_STDIO_H
#define NATIVE_STDIO_H

void NativeStdIOPrintStreamInvoke(uint8_t methodReference);
void NativeStdIOInputStreamInvoke(uint8_t methodReference);
void NativeJavaStringBufferInvoke(uint8_t methodReference);

#endif // NATIVE_STDIO_H
