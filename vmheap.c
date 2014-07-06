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
// heap.c
//
// Heap management

#include "vmtypes.h"
#include "vmerror.h"
#include "vm.h"
#include "utils.h"
#include "vmheap.h"
#include "vmstack.h"

// allocate static heap
uint8_t   heap[HEAPSIZE];
// this is an offset to the base of the available heap.  It changes as the heap is managed.
uint16_t   heapBase = 0;

// ***************************************************************************************************************
// Internal Functions

void HeapCopyMemory(uint8_t   *dst, uint8_t   *src, uint16_t   len) 
{
	dst += len;  
	src += len;
	while(len--) 
  		*--dst = *--src;
}
void HeapShow(void) 
{
  	uint16_t   current = heapBase;

	while(current < sizeof(heap)) 
	{
    	tHeapFrame *h = (tHeapFrame*)&heap[current];
    	if(h->id == kHeapIdFree) 
		{
      
    	} 
		else 
		{
      		if(h->len > sizeof(heap))
				HandleError(kErrorHeapIllegalChunkSize);
		}
	    if(h->len+sizeof(tHeapFrame) > sizeof(heap) - current) 
		{
      		HandleError(kErrorHeapCorrupted);
    	}	
    	current += h->len + sizeof(tHeapFrame);
  	}
}
tHeapFrame   *HeapSearch(tHeapId   id) 
{
  	uint16_t   current = heapBase;

  	while(current < sizeof(heap)) 
	{
    	tHeapFrame   *h = (tHeapFrame*)&heap[current];
    	if(h->id == id) 
			return h;
    	current += h->len + sizeof(tHeapFrame);
  	}
  	return NULL;
}
tHeapId HeapNewId(void) 
{
  	tHeapId   id;

  	for(id=1;id;id++) 
    	if(HeapSearch(id) == NULL) 
      		return id;

  	return 0;
}

BOOL HeapInternalAlloc(tHeapId   id, BOOL   fieldref, uint16_t   size) 
{
  	uint16_t   req = size + sizeof(tHeapFrame);  // total mem required

  	// search for free block
  	tHeapFrame *h = (tHeapFrame*)&heap[heapBase];

  	if(h->len >= req) 
	{
    	// We are going to start pulling new heaps from the end of heap memory so we
		// need to reduce the size of the free block, which is always immediately after the stack
		// to make room.
    	h->len -= req;

	    // The new one is going to be at the end of the free block.  To find the end, we 
		// add up the base of the heap (1+stack bottom) plus the size of the heap frame struct
		// plus the len of the free block.  That gives us the first byte of our newly allocated
		// section.  So we point 'h' to that.
		h = (tHeapFrame*)&heap[heapBase + sizeof(tHeapFrame) + (unsigned int)(h->len)];
		h->id = id;
		h->fieldref = fieldref;
		h->len = size;
#ifdef NVM_INITIALIZE_ALLOCATED
    	// fill memory with zero
    	uint8_t * ptr = (void*)(h+1);
    	while(size--)
      		*ptr++=0;
#endif
    	return TRUE;
  	}

	return FALSE;
}
uint16_t HeapGetLength(tHeapId   id) 
{
  	tHeapFrame   *h = HeapSearch(id);
  	if(!h) 
		HandleError(kErrorHeapChunkMissing);
  	return h->len;
}

BOOL HeapGetFieldReference(tHeapId   id) 
{
	tVmReference   id16 = id | VM_TYPE_HEAP;
	uint16_t   current = heapBase;
	tHeapFrame    *h;

  // walk through the entire heap
  	while(current < sizeof(heap)) 
	{
    	h = (tHeapFrame*)&heap[current];

    	// check for entries with the fieldref flag
    	if(h->fieldref) 
		{
      		uint8_t j;

      		// check all entries in the heap element for
      		// the reference we are searching for
      		for(j=0;j<h->len/sizeof(tVmReference);j++) 
			{
				if(((tVmReference*)(h+1))[j] == id16)
	  			return TRUE;
      		}
    	}

    	current += h->len + sizeof(tHeapFrame);
  	}
  
  	return FALSE;
}

// ***************************************************************************************************************
// Public API


// uint8_t *GetHeapBase(void)
//
// Arguments:
//   void - nothing
//
// Returns:
//   uint8_t* - Actual base of heap ( pointer )
uint8_t   *GetHeapBase(void) 
{
	return heap;
}



#ifdef DEBUG_JVM
// void HeapCheck(void)
//
// Check heap and throw an error if start element is not free
//
// Args;
// Returns:
//   Nothing
void HeapCheck(void) 
{
  	uint16_t   current = heapBase;
  	tHeapFrame   *h = (tHeapFrame*)&heap[current];

  	if(h->id != kHeapIdFree) 
  	{
    	HandleError(kErrorHeapCorrupted);
  	}  
  
  	current += h->len + sizeof(tHeapFrame);

  	while(current < sizeof(heap)) 
  	{
  		h = (tHeapFrame*)&heap[current];
    	if(h->id != kHeapIdFree) 
		{
      		if(h->len > sizeof(heap)) 
			{
	
				HeapShow();
				error(kErrorHeapIllegalChunkSize);
      		}	
    	} 
		else 
		{
    		HeapShow();
      		HandleError(kErrorHeapCorrupted);
    	}
    
    	if(h->len+sizeof(tHeapFrame) > sizeof(heap) - current) 
		{
      		HeapShow();
      		HandleError(kErrorHeapCorrupted);
    	}

    	current += h->len + sizeof(tHeapFrame);	
	}

  	if(current != sizeof(heap)) 
	{
    
    	HeapShow();
    	HandleError(kErrorHeapCorrupted);
  	}
}
#endif

// tHeapId HeapAlloc(BOOL fieldref, uint16_t size)
//
// Allocate a new heap frame and return the id.
//
// Args:
//  bool fieldref - field reference
//  uint16_t size - size to allocate
// Returns;
//  tHeapId Id of heap frame

tHeapId HeapAlloc(BOOL   fieldref, uint16_t   size) 
{
  	tHeapId   id = HeapNewId();

	if(!HeapInternalAlloc(id, fieldref, size)) 
	{
		HeapCollectGarbage();
    	// we need to reallocate heap id, gc. threw away the old one..
    	if(!HeapInternalAlloc(id, fieldref, size))
      		HandleError(kErrorHeapOutOfMemory);
 	 }

  	return id;
}

// void HeapRealloc(tHeapId id, uint16_t size)
//
// Reallocate heap space for an existing frame
//
// Args:
//  tHeapId id - id of existing frame
//  uint16_t size - size to reallocate to
// Returns:
//  nothing
void HeapRealloc(tHeapId   id, uint16_t   size) 
{
	tHeapFrame   *h_new;
	tHeapFrame   *h;

  	// check free mem and call garbage collection if required
  	h = (tHeapFrame*)&heap[heapBase];
  	if(h->len >= size + sizeof(tHeapFrame))
    	HeapCollectGarbage();

  	// get info on old chunk
  	h = HeapSearch(id);

  	// allocate space for bigger one
  	if(!HeapInternalAlloc(id, h->fieldref, size))
    	HandleError(kErrorHeapOutOfMemory);

  	h_new = HeapSearch(id);

  	UtilsCopyMemory(h_new+1, h+1, h->len);

  	h->id = 0xff;  // unused id to make garbage collection delete
                 // this chunk next time
}


// void *HeapGetAddress(tHeapid id)
//
// Get the physical address in RAM for the start of a heap frame
//
// Args;
//  tHeapId id - id of heap frame
// Returns:
//  void* pointer to heap frame start in RAM
void   *HeapGetAddress(tHeapId   id) 
{
  tHeapFrame   *h = HeapSearch(id);
  if(!h) HandleError(kErrorHeapChunkMissing);
  // we need to get past the heap frame to the first byte of the heap.
  // By adding 1 we are really adding sizeof(tHeapFrame)
  h+=1;
  return h;
}
// void HeapInitialize(void)
//
// Initialize the heap
// 
// Args:
// Returns:
//  nothing
void HeapInitialize(void) 
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	//DEBUGF("heap_init()\n");

	// just one big free block
	tHeapFrame   *h = (tHeapFrame   *)&heap[0];
	h->id  = (tHeapId)kHeapIdFree;
	a = sizeof(heap);
	b = sizeof(tHeapFrame);
	c=a-b;
	//heap[0] = 0;
	//heap[1] = 0;
	//heap[4] = c & 0xff;
	//heap[5] = (c>>8) &0xff;
	h->fieldref = FALSE;
	h->len = c;

}

// void HeapCollectGarbage(void)
//
// Walk the heap and dump any objects not being used
//
// Args:
// Returns:
//   nothing
void HeapCollectGarbage(void) 
{
  	uint16_t   current = heapBase;
  	uint16_t   len;
  	tHeapFrame   *h;
	// set current to stack-top
  	// walk through the entire heap
  	while(current < sizeof(heap)) 
	{
    	h = (tHeapFrame  *)&heap[current];
    	len = h->len + sizeof(tHeapFrame);

    	// found an entry
    	if(h->id != kHeapIdFree) 
		{
      		// check if it's still used
      		if((!StackIsHeapIdInUse(h->id))&&(!HeapGetFieldReference(h->id))) 
			{
				// it is not used, remove it
      
				// move everything before to the top
				HeapCopyMemory(heap+heapBase+len, heap+heapBase, current-heapBase);

				// add freed mem to free-chunk
				h = (tHeapFrame*)&heap[heapBase];
				h->len += len;
      		}
    	}
    	current += len;
  	}

  	if(current != sizeof(heap)) 
	{
    	HandleError(kErrorHeapCorrupted);
  	}
}


// void HeapSteal(uint16_t byteCount)
//
// Steal bytes from the base of the heap
//
// Args:
//  uint16_t byteCount - number of bytes to steal
// Returns:
//  Nothing
void HeapSteal(uint16_t   byteCount) 
{
  	tHeapFrame   *h = (tHeapFrame*)&heap[heapBase];
  	uint16_t   len;

  	if(h->id != kHeapIdFree) 
	{
   		HandleError(kErrorHeapCorrupted);
  	}

	// try to make space if necessary
	len = h->len;
	if(len < byteCount) 
    	HeapCollectGarbage();

	len = h->len;
	if(len < byteCount) 
		HandleError(kErrorHeapOutOfStackMemory);

	// finally steal ...
	heapBase += byteCount;
	h = (tHeapFrame  *)&heap[heapBase];
	h->id = kHeapIdFree;
	h->fieldref=0;
	h->len = len - byteCount;
}

// void HeapUnsteal(uint16_t byteCount)
//
// Return bytes to the heap
//
// Args:
//  uint16_t byteCount - number of bytes
// Returns:
//  nothing
void HeapUnsteal(uint16_t   byteCount) 
{
  	tHeapFrame   *h = (tHeapFrame*)&heap[heapBase];
  	uint16_t   len;

  	if(h->id != kHeapIdFree) 
	{    
    	HandleError(kErrorHeapCorrupted);
  	}

  	if(heapBase < byteCount) 
	{
        HandleError(kErrorHeapStackUnderrun);
  	}

	  // finally unsteal ...
	  len = h->len;
	  heapBase -= byteCount;
	  h = (tHeapFrame  *)&heap[heapBase];
	  h->id = kHeapIdFree;
	  h->len = len + byteCount;
}

