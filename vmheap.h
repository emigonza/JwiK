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
// heap.h


#ifndef VMHEAP_H
#define VMHEAP_H

#if HEAPSIZE <= 1024
typedef uint8_t tHeapId;
#else
typedef uint16_t tHeapId;
#endif 

typedef enum
{
	kHeapIdFree=0
} HeapIdTypes;

// Fixed this.  In the 8051, you cannot use uint8_t fields in a struct.
// fieldref is its own int now and len uses all bits.
typedef struct 
{
	tHeapId id;
	unsigned int fieldref;
	unsigned int len;
} tHeapFrame;

void HeapInitialize(void);
uint8_t   *GetHeapBase(void);
tHeapId HeapAlloc(BOOL fieldref, uint16_t size);
void HeapRealloc(tHeapId id, uint16_t size);
uint16_t HeapGetLength(tHeapId id);
void   *HeapGetAddress(tHeapId id);
//hey, this is java!!!  void      heap_free(tHeapId id);
void HeapCollectGarbage(void);
void HeapSteal(uint16_t bytes);
void HeapUnsteal(uint16_t bytes);
tHeapFrame   *HeapSearch(tHeapId   id);
#ifdef DEBUG_JVM
void HeapCheck(void);
#define HeapCheck()  HeapCheck()
#else
#define HeapCheck() 
#endif

#endif // VMHEAP_H
