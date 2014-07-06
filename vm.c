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
// vm.c
//
// Java VM for small microcontrollers implementation source code

//#include "config.h"
#include "vmerror.h"

#include "vm.h"
#include "opcodes.h"

// include the platform dependent native files based on the selected platform
#include "native_java.h"

#include "vmstack.h"
#include "vmfeatures.h"

#ifdef VM_INCLUDE_ARRAY
#include "vmarray.h"
#endif

#ifdef VM_INCLUDE_32BIT_WORD
# define DBG_INT "0x" DBG32
#else
# define DBG_INT "0x" DBG16
#endif
// pc/methodref/localsoffset
#define VM_METHOD_CALL_REQUIREMENTS 3

// we prefetch arguments from the program storage
// and this is the type it is stored into
typedef union 
{
  	int16_t w;
  	struct 
	{
    	//int8_t bl, bh;
		// since we are big endian, this must be:;
		int8_t bh, bl;
  	} z;
  	tVmInt tmp;
} vm_arg_t;

tVmStack   *locals;

// *******************************************************************************************************
// Public API

// void VmInitialize(void)
// 
// Initializes virtual machine
//
// Args:
// Returns:
//  nothing
void VmInitialize(void) 
{
	// init heap
	HeapInitialize();

	// get stack space from heap and setup stack
	StackInitialize(VmFileGetStaticFieldCount());

	// args parameter to main (should be a string array)
	StackPush(0); 
}
// void *VmGetAddress(tVmReference ref) 
//
// Get the address of a referenced object from RAM or the VmFIle in flash.
//
// Args:
//  tVmReference ref - reference to object
void *VmGetAddress(tVmReference ref) 
{
  	if(!(ref & VM_IMMEDIATE_MASK))
    	HandleError(kErrorVmIllegalReference);

  	if((ref & VM_TYPE_MASK) == VM_TYPE_HEAP)
    	return HeapGetAddress(ref & ~VM_TYPE_MASK);

  	// return nvmfile address and set marker indicating
  	// that this is inside the nvm file (and may have
  	// to be accessed in a special manner)
  	return VMSETFILEFLAG(VmFileGetAddress(ref & ~VM_TYPE_MASK));
}
// tVmInt VmStackToInt(tVmStack val) 
//
// expand 15 uint8_t immediate from stack to 16 bits (or 31 to 32)
//
// Args:
//  tVmStack val - stack value to convert
// Returns:
//  tVmInt - resulting 16 uint8_t integer
tVmInt VmStackToInt(tVmStack val) 
{
  	if(val & (VM_IMMEDIATE_MASK>>1))
    	val |= VM_IMMEDIATE_MASK;   // expand sign bit
  	return val;
}

#ifdef VM_INCLUDE_FLOAT
// tVmStack VmFloatToStack(tVmFloat val)
//
// convert a floating point value to a stack value
//
// Args:
//  tVmFloat val - value to convert
// Returns:
//  tVmStack - stack value
tVmStack VmFloatToStack(tVmFloat val)
{
	tVmUnion   v;
	uint8_t   msb = (v.b[3]&0x80)?0x40:0x00;

	v.f[0]=val;
	v.b[3] &= 0x7f;
	if (v.b[3]==0x7f && (v.b[2]&0x80)==0x80)
		msb |= 0x3f;
	else if (v.b[3]!=0x00 || (v.b[2]&0x80)!=0x00)
		msb |= v.b[3]-0x20;
	v.b[3]=msb;
	return v.i[0];
}
// tVmFloat VmStackToFloat(tVmStack val)
//
// Convert a stack value to a floating point value
//
// Args:
//  tVmStack val - value to convert
// Returns:
//  tVmFloat - float result
tVmFloat VmStackToFloat(tVmStack val)
{
	tVmUnion   v;
	uint8_t   msb = (v.b[3]&0x40)?0x80:0x00;

	v.i[0]=val;
	v.b[3] &= 0x3f;
	if (v.b[3]==0x3f && (v.b[2]&0x80)==0x80)
		msb |= 0x7f;
	else if (v.b[3]!=0x00 || (v.b[2]&0x80)!=0x00)
		msb |= v.b[3]+0x20;
	v.b[3]=msb;
	return v.f[0];
}

#endif

// tHeapId VmNewClass(uint16_t classReference)
//
// Creates a new class using its reference and returns the class' heapid
//
// Args:
//  uint16_t classReference - reference of class to create
// Returns:
//  tHeapId - id of object on the heap
tHeapId VmNewClass(uint16_t classReference) 
{
	tHeapId heapId;
	tVmReference *heapAddress;

  	if(NATIVE_ID2CLASS(classReference) < NATIVE_CLASS_BASE) 
  	{
		// This stackpop should not be needed.  In my test program, the <init> for the program
		// class was failing to return because the stack was not empty.  The instructions were
		// as follows:
		//
		// ILOAD_0
		// NEW 0x0100
		// DUP
		// INVOKESPECIAL 0x0003
		// PUTFIELD 0x0000
		// RETURN
		//
		// The ILOAD_0 before the NEW was pushing the class reference onto the stack but 
		// no one was popping it off.  Technically, there is supposed to be nothing on the 
		// stack when NEW is called that is required by NEW.  However, I could find no 
		// other instruction that references the stack entry made by that first ILOAD_0.
		// NEW leaves a reference to the new instance on the stack.  DUP duplicates it.
		// INVOKESPECIAL calls the init function of the new class, which is stack neutral.
		// PUTFIELD pops the first reference from the stack to know where to access it
		// and then pops the second reference and stores it in the 'this' self reference var
		// on the new instance's heap. This accounts for the pushes by NEW and DUP.  At
		// this point, the ILOAD_0's push is still on the stack and it is time to exit the
		// method execution.  It seems like the ILOAD_0 is superfluous, but it is being added
		// by the compiler, so there is some reason.
		//
		// 9-26-2012
		// It turns out, that this is the wrong place.  The extra reference is supposed to be 
		// consumed by the invocation of the new classes default constructor.
		//StackPop();
    
		// create object with
    	heapId = HeapAlloc(TRUE, sizeof(tVmWord) *
    		 (VM_CLASS_CONST_ALLOC+VmFileGetClassFieldCount(NATIVE_ID2CLASS(classReference))*sizeof(uint32_t)));

		// todo:
		//
		// It would actually make more sense to push the actual pointer to the heap
		// frame for this instance instead of the ID.  That way, we could address the memory 
		// directly without a heapId lookup.  This will fundamentally change how we access
		// the heap and all calls to HeapGetAddress will have to be refactored so that 
		// the value passed is the actual address, not the heap ID.  To do this, we need to eval
		// every call to HeapGetAddress and make sure the new method will work as expected.
   	 	StackPush(VM_TYPE_HEAP | heapId);

    	// store reference in object, so we can later determine which kind
    	// of object this is. this is required for inheritance
		heapAddress = HeapGetAddress(heapId);
		// the first local in any instance heap frame is going to be a class reference.
    	heapAddress[0] = classReference;

    	return heapId;
  	}

  	return CreateNewNativeObject(classReference);
}




void   VmRunMethod(uint16_t mref) 
{
	uint8_t   instr, pc_inc, *pc, argh, argl;
	
	tVmStack   stackPopVal;
	tVmStack   *a;
	tVmStack   *c;
	uint16_t   b;
	tVmInt   tmp1=0;
	tVmInt   tmp2;
	vm_arg_t   arg0;
	tVmFileMethodHeader   mhdr, *mhdr_ptr;
	
	uint8_t   size;
	uint32_t lv;

#ifdef VM_INCLUDE_FLOAT
	tVmFloat f0;
	tVmFloat f1;
#endif

	// load method header into ram
	mhdr_ptr = VmFileGetMethodHeader(mref);
	VmFileRead(&mhdr, mhdr_ptr, sizeof(tVmFileMethodHeader));

	// determine method description address and code
	pc = (uint8_t*)mhdr_ptr + mhdr.code_index;


	// increase stack space. locals will be put on the stack as 
	// well. method arguments are part of the locals and are 
	// already on the stack
	HeapSteal(sizeof(tVmStack) * (mhdr.max_locals + mhdr.max_stack + mhdr.args));

	// determine address of current locals (stack pointer + 1)
	locals = GetStackPointer();// + 1;
	
	StackAddToSp(mhdr.max_locals-1);
	StackSaveBase();

	do 
	{
    	instr = VmFileRead08(pc);
    	pc_inc = 1;
    	argh =  VmFileRead08(pc+1);
    	arg0.z.bh = argh;
		argl = VmFileRead08(pc+2);
    	arg0.z.bl = argl;

		switch(instr)
		{
			case OP_NOP:
				// don't know why, but we are wasting an instruction.  Seems silly in a VM
				break;
			case OP_BIPUSH:
				StackPush(arg0.z.bh); pc_inc = 2;
				break;
			case OP_SIPUSH:
				StackPush(~VM_IMMEDIATE_MASK & (arg0.w)); pc_inc = 3;
				break;
			case OP_ICONST_M1:
			case OP_ICONST_0:
			case OP_ICONST_1:
			case OP_ICONST_2:
			case OP_ICONST_3:
			case OP_ICONST_4:
			case OP_ICONST_5:
				StackPush(instr - OP_ICONST_0);
				break;
			case OP_ISTORE:
				locals[arg0.z.bh] = StackPop(); pc_inc = 2;
				break;
			case OP_ISTORE_0:
			case OP_ISTORE_1:
			case OP_ISTORE_2:
			case OP_ISTORE_3:
				stackPopVal = StackPop();
				tmp1 = instr - OP_ISTORE_0;
				*(locals+tmp1) = stackPopVal;

				break;
			case OP_ILOAD:
				StackPush(locals[arg0.z.bh]); pc_inc = 2;
				break;
			case OP_ILOAD_0:
			case OP_ILOAD_1:
			case OP_ILOAD_2:
			case OP_ILOAD_3:
				StackPush((tVmReference)locals[instr - OP_ILOAD_0]);
				break;
			case OP_IFEQ:
			case OP_IFNE:
			case OP_IFLT:
			case OP_IFGE:
			case OP_IFGT:
			case OP_IFLE:
			case OP_IF_ICMPEQ:
			case OP_IF_ICMPNE:
			case OP_IF_ICMPLT:
			case OP_IF_ICMPGE:
			case OP_IF_ICMPGT:
			case OP_IF_ICMPLE:
				if((instr >= OP_IFEQ) && (instr <= OP_IFLE)) 
				{
					// comparision with zero
					tmp2 = 0;
					instr -= OP_IFEQ - OP_IF_ICMPEQ;
		      	}
				else 
				{
					// comparison with second argument
					tmp2 = StackPopInt();
		      	}

		      	tmp1 = StackPopInt();

		      	switch(instr) 
				{
			        case OP_IF_ICMPEQ: 
			          	tmp1 = (tmp1 == tmp2); 
					  	break;
			        case OP_IF_ICMPNE: 
			          	tmp1 = (tmp1 != tmp2); 
						break;
			        case OP_IF_ICMPLT: 
			          	tmp1 = (tmp1 <  tmp2); 
						break;
			        case OP_IF_ICMPGE: 
			          	tmp1 = (tmp1 >= tmp2); 
						break;
			        case OP_IF_ICMPGT: 
			          	tmp1 = (tmp1 >  tmp2); 
						break;
			        case OP_IF_ICMPLE: 
			          	tmp1 = (tmp1 <= tmp2); 
						break;
		      	}
				break;
			case OP_GOTO:
			    pc_inc = 3;
      			pc += (arg0.w-3);
				break;
			case OP_IADD:
			case OP_FADD:
			case OP_ISUB:
			case OP_FSUB:
			case OP_IMUL:
			case OP_FMUL:
			case OP_IDIV:
			case OP_FDIV:
			case OP_IREM:
			case OP_FREM:
			case OP_INEG:
			case OP_FNEG:
			case OP_ISHL:
			case OP_ISHR:
			case OP_IUSHR:
			case OP_IAND:
			case OP_IOR:
			case OP_IXOR:
			case OP_IINC:
			     	// single operand arithmetic
		      	if(instr == OP_INEG) 
				{
					tmp1 = -StackPopInt();
					StackPush(nvm_int2stack(tmp1));
		      	} 
				else if(instr == OP_IINC) 
				{
					locals[arg0.z.bh] = (VmStackToInt(locals[arg0.z.bh]) + arg0.z.bl) 
			  								& ~VM_IMMEDIATE_MASK; 
					pc_inc = 3;

#ifdef VM_INCLUDE_FLOAT
		      	} 
				else if(((instr & 0x03) == 0x02) && (instr <= OP_FNEG)) 
				{
		        	if (instr == OP_FNEG) 
					{
		          		f0 = -StackPopFloat();
		         	 	StackPush(VmFloatToStack(f0));
		        	}
		        	else 
					{
		          		f0 = StackPopFloat();  // fetch operands from stack
		          		f1 = StackPopFloat();
		          		switch(instr) 
						{
		            		case OP_FADD:
		              			f1  += f0; 
								break;
		            		case OP_FSUB:
		              			f1  -= f0; 
								break;
		            		case OP_FMUL:
		              			f1  *= f0; 
								break;
		            		case OP_FDIV:
		             			if(!f0) 
									HandleError(kErrorVmDivisionByZero);
		              			f1  /= f0; 
								break;
		            		case OP_IREM:
		              			HandleError(kErrorVmUnsupportedOpCode);
		              			//f1  = f1%f0; break;
		          		}
		          		StackPush(VmFloatToStack(f1));
          
		        	}
#endif

		      	} 
			  	else 
				{
			  		tmp1 = StackPopInt();  // fetch operands from stack
					tmp2 = StackPopInt();
	
					switch(instr) 
					{
						case OP_IADD:
							tmp2  += tmp1; 
							break;
						case OP_ISUB:
							tmp2  -= tmp1; 
							break;
						case OP_IMUL:
							tmp2  *= tmp1; 
							break;
						case OP_IDIV:
							if(!tmp1) HandleError(kErrorVmDivisionByZero);
								tmp2  /= tmp1; 
							break;
						case OP_IREM:
							tmp2  %= tmp1; 
							break;
						case OP_ISHL:
							tmp2 <<= tmp1; 
							break;
						case OP_ISHR:
							tmp2 >>= tmp1; 
							break;
						case OP_IAND:
							tmp2  &= tmp1; 
							break;
						case OP_IOR: 
							tmp2  |= tmp1; 
							break;
						case OP_IXOR:
							tmp2  ^= tmp1; 
							break;
						case OP_IUSHR:
							tmp2 = ((tVmUInt)tmp2 >> tmp1); 
							break;
					}
	
					// and finally push result
		        	StackPush(nvm_int2stack(tmp2));
		      	}
				break;
			case OP_IRETURN:
#ifdef VM_INCLUDE_FLOAT
			case OP_FRETURN:
#endif
			case OP_RETURN:
				
  				if((instr == OP_IRETURN)
#ifdef VM_INCLUDE_FLOAT
   					||(instr == OP_FRETURN)
#endif
  					) 
				{
					tmp1 = StackPop();     // save result
  				}
				// return from locally called method? other case: return
				// from main() -> end of program
				if(!StackIsEmpty()) 
				{
					uint8_t old_locals = mhdr.max_locals;
					uint8_t old_unsteal = VM_METHOD_CALL_REQUIREMENTS +
										mhdr.max_locals + 
										mhdr.max_stack + 
										mhdr.args;

					uint16_t old_localsoffset = StackPop();

					// make space for locals on the stack

					mref = StackPop();

					// read header of method to return to
					mhdr_ptr = VmFileGetMethodHeader(mref);
					// load method header into ram
					VmFileRead(&mhdr, mhdr_ptr, sizeof(tVmFileMethodHeader));

					// restore pc
					pc = (uint8_t*)mhdr_ptr + StackPop();
					pc_inc = 3; // continue _behind_ calling 3 instruction

					// and remove locals from stack and hope that method left
					// an uncorrupted stack

					// todo:
					// This must be wrong because we are advancing the stack by the arg count, not
					// local count in the invoke call.

					StackAddToSp(-old_locals);
					a=GetStackPointer();
					b=old_localsoffset;
					locals = (tVmStack  *) ((uint16_t)a-b);

					// give memory used by returning method back to heap
					HeapUnsteal(sizeof(tVmStack) * old_unsteal);

					if(instr == OP_IRETURN)
					{
						StackPush(tmp1);
    				}
#ifdef VM_INCLUDE_FLOAT
    				else if(instr == OP_FRETURN)
					{
  						StackPush(tmp1);
					}
#endif
					instr = OP_NOP;  // make vm continue
  				}
				break;
			case OP_POP2:
				StackPop(); 
				StackPop();
				break;
			case OP_POP:
				StackPop();
				break;
			case OP_DUP:
				StackPush(StackPeek(0));
				break;
			case OP_DUP2:
				StackPush(StackPeek(1));
				StackPush(StackPeek(1));
				break;
#ifdef VM_INCLUDE_EXTSTACKOPS
			case OP_DUP_X1:
				tVmStack w1 = StackPop();
	      		tVmStack w2 = StackPop();
      			StackPush(w1);
      			StackPush(w2);
      			StackPush(w1);
				break;
			case OP_DUP_X2:
				tVmStack w1 = StackPop();
				tVmStack w2 = StackPop();
				tVmStack w3 = StackPop();
				StackPush(w1);
				StackPush(w2);
				StackPush(w3);
				StackPush(w1);
				break;
			case OP_DUP2_X1:
				tVmStack w1 = StackPop();
				tVmStack w2 = StackPop();
				tVmStack w3 = StackPop();
				StackPush(w1);
				StackPush(w2);
				StackPush(w3);
				StackPush(w1);
				StackPush(w2);
				break;
			case OP_DUP2_X2:
				tVmStack w1 = StackPop();
				tVmStack w2 = StackPop();
				tVmStack w3 = StackPop();
				tVmStack w4 = StackPop();
				StackPush(w1);
				StackPush(w2);
				StackPush(w3);
				StackPush(w4);
				StackPush(w1);
				StackPush(w2);
				break;
			case OP_SWAP:
				tVmStack w1 = StackPop();
				tVmStack w2 = StackPop();
				StackPush(w1);
				StackPush(w2);
				break;
#endif    
#ifdef VM_INCLUDE_TABLESWITCH
			case OP_TABLESWITCH:
				tmp1 = ((VmFileRead08(pc+7)<<8) |
				VmFileRead08(pc+8));        // get low value
				tmp2 = ((VmFileRead08(pc+11)<<8) |
				VmFileRead08(pc+12));       // get high value
				arg0.tmp = StackPop();               // get actual value
				//DEBUGF("tableswitch %d-%d (%d)\n", tmp1, tmp2, arg0.w);

				// value within range?
				if((arg0.tmp < tmp1)||(arg0.tmp > tmp2))
				// no: use default
				tmp2 = 3;
				else
				// yes: get offset from table
				tmp2 = 3 + 12 + ((arg0.tmp - tmp1)<<2);

				// and do the jump
				pc += ((VmFileRead08(pc+tmp2+0)<<8) | 
				VmFileRead08(pc+tmp2+1));
				pc_inc = 0;
				break;
#endif
#ifdef VM_INCLUDE_LOOKUPSWITCH
			case OP_LOOKUPSWITCH:
				arg0.tmp = 1 + 4;
				size = VmFileRead08(pc+arg0.tmp+3); // get table size (max for nvm is 30 cases!)
				arg0.tmp += 4;
				tmp1 = StackPopInt();                        // get actual value
				while(size)
				{
					if (
#ifdef VM_INCLUDE_32BIT_WORD
				     	VmFileRead08(pc+arg0.tmp+0)==(uint8_t)(tmp1>>24) &&
				     	VmFileRead08(pc+arg0.tmp+1)==(uint8_t)(tmp1>>16) &&
#endif
				     	VmFileRead08(pc+arg0.tmp+2)==(uint8_t)(tmp1>>8) &&
				     	VmFileRead08(pc+arg0.tmp+3)==(uint8_t)(tmp1>>0)
				   		)
					{
				  		arg0.tmp+=4;
				  		break;
					}
					arg0.tmp+=8;
					size--;
				}
				if (size==0)
				{
					arg0.tmp = 1;
				}
				pc += ((VmFileRead08(pc+arg0.tmp+2)<<8) |
				     VmFileRead08(pc+arg0.tmp+3));
				pc_inc = 0;
				break;
#endif
			case OP_GETSTATIC:
				pc_inc = 3;   // prefetched data used
				StackPush(StackGetStaticVariable(arg0.w));
				break;
			case OP_PUTSTATIC:
				pc_inc = 3;
				StackSetStaticVariable(arg0.w, StackPop());
				break;
			case OP_LDC:
				pc_inc = 2;
				//DEBUGF("ldc #"DBG16"\n", arg0.z.bh);
#ifdef VM_INCLUDE_32BIT_WORD
				StackPush(VmFileGetConstant(arg0.z.bh));
#else
				StackPush(VM_TYPE_CONST | (arg0.z.bh-vmFileConstantCount));
#endif
				break;
			case OP_INVOKEVIRTUAL:
			case OP_INVOKESPECIAL:
			case OP_INVOKESTATIC:
				if(arg0.z.bh < NATIVE_CLASS_BASE) 
				{
					// save current pc (relative to method start)
					tmp1 = (uint8_t*)pc-(uint8_t*)mhdr_ptr;

					// get pointer to new method
					mhdr_ptr = VmFileGetMethodHeader(arg0.w);

					// load new method header into ram
					VmFileRead(&mhdr, mhdr_ptr, sizeof(tVmFileMethodHeader));

					#ifdef VM_INCLUDE_INHERITANCE
					// check class on stack. it may be not the one we expect.
					// this happens due to inheritance
					if(instr == OP_INVOKEVIRTUAL) 
					{ 

						// fetch class reference from stack and use it to address
						// the class instance on the heap. The first entry in this 
						// object is the class id of it
						//
						// 9/26/2012
						// 
						// This used to be:
						//
						// tVmReference mref = ((tVmReference*)HeapGetAddress(StackPeek(0) & ~VM_TYPE_MASK))[0]
						// if(NATIVE_ID2CLASS(mref) != NATIVE_ID2CLASS(mhdr.id)) 
						// { .... }
						//
						// This won't work because taking a tVmReference* pointer and dereferencing it
						// by using an array operator '[0]' will cast the result to an uint16_t, not a 
						// uint32_t.  The following code splits out the references and casts the 
						// dereferenced value to a uint32_t.  Before this change, the equality would 
						// always fail.  Now, it works fine.
						tVmReference  *classRef = (tVmReference*)HeapGetAddress(StackPeek(0) & ~VM_TYPE_MASK);
						tVmReference aref = NATIVE_ID2CLASS((tVmReference)classRef[0]);
						tVmReference cref =  NATIVE_ID2CLASS(mhdr.id);
						
						if(aref != cref) 
						{

							// get matching method in class on stack or its
							// super classes
							arg0.z.bl = VmFileGetMethodByClassAndId(
							  NATIVE_ID2CLASS(mref), NATIVE_ID2METHOD(mhdr.id));

							// get pointer to new method
							mhdr_ptr = VmFileGetMethodHeader(arg0.z.bl);

							// load new method header into ram
							VmFileRead(&mhdr, mhdr_ptr, sizeof(tVmFileMethodHeader));
						}
					}
					#endif

					// at this point, the stack should have the new method's 
					// local arguments at its top.  What we are doing here is
					// to back up the stack pointer so that these arguments
					// will be at the bottom of the new stack frame when we steal
					// memory from the heap.  Then, we will advance the SP back to 
					// its original spot and the arguments will be in the locals
					// section of the stack frame.

					// Before:
					// 
					// Current stack frame       --- TOS
					// arg 0, arg 1, arg 2, .. arg n
					//                               ^ stack pointer

					// After:
					//
					// TOS
					//    arg 0, arg 1.....arg n
					// ^ stack pointer

					// todo:
					// This must be wrong because we are advancing the stack by the max_locals
					// count, not arg count in the return.  Also, we need to make sure we consume
					// the ILOAD_0 because it does not get popped off the stack anywhere else.


					StackAddToSp(-(mhdr.args));
					
					// tmp2 is equal to the new stack pointer minus the position of 
					// the locals.
					//
					// Example:
					//    Given sp=0x001F and locals=0x0010
					//  ==> tmp2 = 0x000F
					a= GetStackPointer();

					// this little uint8_t of voodoo is required to 
					// make the C51 compiler do the math right.  For
					// other platforms, we might put this in a ifdef
					// block and use a direct formula if their compiler
					// is not stupid.
					b= locals+1;
					c=(tVmStack   *)((uint16_t)a-b);
					tmp2 = (uint32_t)c;
					
					// Now we need to point the locals array to the new locals position which 
					// is one past the current stack pointer
					locals = GetStackPointer();// + 1;


					// Now we need to steal space on the heap for the new method's
					// stack frame.  This will be put in the first available free block
					// and will require enough 4 byte stack entries to account for 
					// the method call requirements (pc offset, method reference, and 
					// locals offset) plus the maximum number of locals plus the the
					// maxmium stack requirements plus the number of arguments (which
					// are already on the old stack frame.

					HeapSteal(sizeof(tVmStack) *
						   (VM_METHOD_CALL_REQUIREMENTS +
						    mhdr.max_locals + mhdr.max_stack + mhdr.args));

					// add space for locals on stack
					StackAddToSp(mhdr.max_locals-1);

					// push everything required to return onto the stack
					StackPush(tmp1);   // pc offset
					StackPush(mref);   // method reference
					StackPush(tmp2);   // locals offset

					// set new pc (this is the actual call)
					mref = arg0.w;
					pc = (uint8_t*)mhdr_ptr + mhdr.code_index;
					pc_inc = 0;  // don't add further bytes to program counter
				} 
				else 
				{ 
					InvokeNativeMethod(arg0.w);
					pc_inc = 3;   // prefetched data used
				}
				break;
			case OP_GETFIELD:
				pc_inc = 3;
				stackPopVal = StackPop() & ~VM_TYPE_MASK;
				StackPush( ((tVmWord*)HeapGetAddress(stackPopVal))[VM_CLASS_CONST_ALLOC+arg0.w] );
				break;
			case OP_PUTFIELD:
				// this op code uses both arguments, so increment the pc by 3
				pc_inc = 3;
				// right now the stack should have the value to put in the field on top and
				// the heap id for our class.  We will then use that heap id to get the real
				// in memory of the heapframe and index into it using arg0. to write our value.
				tmp1 = StackPop();
				stackPopVal = StackPop();
				((tVmWord*)HeapGetAddress(stackPopVal & ~VM_TYPE_MASK))[VM_CLASS_CONST_ALLOC+arg0.w] = tmp1;
				break;
			case OP_NEW:
				pc_inc = 3;
				VmNewClass(arg0.w);
				break;
#ifdef VM_INCLUDE_ARRAY
			case OP_NEWARRAY:
				pc_inc = 2;
				StackPush(ArrayNew(StackPop(), arg0.z.bh) | VM_TYPE_HEAP);
				break;
			case OP_ARRAYLENGTH:
				StackPush(ArrayLen(StackPop() & ~VM_TYPE_MASK));
				break;
			case OP_BASTORE:
				tmp2 = StackPopInt();       // value
				tmp1 = StackPopInt();         // index
				// third parm on stack: array reference
				ArraySetByteValue(StackPop() & ~VM_TYPE_MASK, tmp1, tmp2);
				break;
			case OP_IASTORE:
				tmp2 = StackPopInt();       // value
				tmp1 = StackPopInt();       // index
				// third parm on stack: array reference
				ArraySetIntValue(StackPop() & ~VM_TYPE_MASK, tmp1, tmp2);
				break;
			case OP_BALOAD:									
				tmp1 = StackPopInt();       // index
				// second parm on stack: array reference
				StackPush(ArrayGetByteValue(StackPop() & ~VM_TYPE_MASK, tmp1));
				break;
			case OP_IALOAD:
				tmp1 = StackPopInt();       // index
				// second parm on stack: array reference
				StackPush(ArrayGetIntValue(StackPop() & ~VM_TYPE_MASK, tmp1));
				break;
#endif
#ifdef VM_INCLUDE_OBJ_ARRAY
			case OP_ANEWARRAY:		
				// Object array is the same as int array...
				pc_inc = 3;
				StackPush(ArrayNew(StackPop(), T_INT) | VM_TYPE_HEAP);
				break;
			case OP_AASTORE:
				tmp2 = StackPopInt();       // value
				tmp1 = StackPopInt();       // index
				// third parm on stack: array reference
				ArraySetIntValue(StackPop(), tmp1, tmp2);
				break;
			case OP_AALOAD:
				tmp1 = StackPopInt();       // index
				// second parm on stack: array reference
				StackPush(ArrayGetIntValue(StackPop(), tmp1));
				break;
#endif
#ifdef VM_INCLUDE_FLOAT
#ifdef VM_INCLUDE_ARRAY
			
			case OP_FALOAD:
				tmp1 = StackPopInt();       // index
				// second parm on stack: array reference
				StackPush(ArrayGetFloatValue(StackPop() & ~VM_TYPE_MASK, tmp1));
				break;
			case OP_FASTORE:
				f0 = StackPopFloat();       // value
				tmp1 = StackPopInt();         // index
				// third parm on stack: array reference
				ArraySetFloatValue(StackPop() & ~VM_TYPE_MASK, tmp1, f0);
				break;
#endif
			case OP_FCONST_0:
				StackPush(VmFloatToStack(0.0));
				break;
			case OP_FCONST_1:
				StackPush(VmFloatToStack(1.0));
				break;
			case OP_FCONST_2:
				StackPush(VmFloatToStack(2.0));
				break;
			case OP_I2F:
				tmp1 = StackPopInt();
				StackPush(VmFloatToStack(tmp1));
				break;
			case OP_F2I:
				tmp1 = StackPopFloat();
				StackPush(nvm_int2stack(tmp1));
				break;
			case OP_FSTORE:
				locals[arg0.z.bh] = StackPop(); 
				pc_inc = 2;
				break;
			case OP_FSTORE_0:
			case OP_FSTORE_1:
			case OP_FSTORE_2:
			case OP_FSTORE_3:
				locals[instr - OP_FSTORE_0] = StackPop();
				break;
			case OP_FLOAD:
				StackPush(locals[arg0.z.bh]); pc_inc = 2;
				break;
			case OP_FLOAD_0:
			case OP_FLOAD_1:
			case OP_FLOAD_2:
			case OP_FLOAD_3:
				StackPush(locals[instr - OP_FLOAD_0]);
				break;
			case OP_FCMPL:
			case OP_FCMPG:
				f1 = StackPopFloat();
				f0 = StackPopFloat();
				tmp1=0;
				if (f0<f1)
					tmp1=-1;
				else if (f0>f1)
					tmp1=1;
				StackPush(nvm_int2stack(tmp1));
				break;
#endif
			default:
				HandleError(kErrorVmUnsupportedOpCode);

		}
		// reset watchdog here if present

    	pc += pc_inc;
		
 	} while((instr != OP_IRETURN)&&(instr != OP_RETURN));

	// and remove locals from stack and hope that method left
	// an uncorrupted stack
	StackAddToSp(-mhdr.max_locals);

	// give memory back to heap
	HeapUnsteal(sizeof(tVmStack) * (mhdr.max_locals + mhdr.max_stack + mhdr.args));
}

