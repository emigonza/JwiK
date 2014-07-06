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
// native_Math.c
//
// Native Java Math Object and Methods

#include "platform_types.h"
#include "config.h"
#include "vmerror.h"

#ifdef VM_INCLUDE_MATH


#include "native_math.h"
#include "native_java.h"
#include "vmstack.h"
#include "vm.h"
#include <math.h>
#include <stdlib.h>


#define NATIVE_METHOD_absF 1
#define NATIVE_METHOD_absI 2
#define NATIVE_METHOD_acos 3
#define NATIVE_METHOD_asin 4
#define NATIVE_METHOD_atan 5
#define NATIVE_METHOD_atan2 6
#define NATIVE_METHOD_ceil 7
#define NATIVE_METHOD_cos 8
#define NATIVE_METHOD_exp 9
#define NATIVE_METHOD_floor 10
#define NATIVE_METHOD_log 11
#define NATIVE_METHOD_maxF 12
#define NATIVE_METHOD_maxI 13
#define NATIVE_METHOD_minI 14
#define NATIVE_METHOD_minF 15
#define NATIVE_METHOD_pow 16
#define NATIVE_METHOD_random 17
#define NATIVE_METHOD_rint 18
#define NATIVE_METHOD_round 19
#define NATIVE_METHOD_sin 20
#define NATIVE_METHOD_sqrt 21
#define NATIVE_METHOD_tan 22
#define NATIVE_METHOD_toDegrees 23
#define NATIVE_METHOD_toRadians 24
#define M_PI 3.14159265

// ****************************************************************************************************************
// Public API


// void NativeMathInvoke(uint8_t methodReference)
//
// Invoke a native method in the Math object
//
// Args:
//  uint8_t methodReference - reference of method to invoke
//
// Returns:
//  Nothing
void NativeMathInvoke(uint8_t methodReference) 
{
	tVmFloat a;
	tVmFloat b;
	tVmInt ai;
	tVmInt bi;

	switch(methodReference)
	{
		case NATIVE_METHOD_absF:
			StackPush(VmFloatToStack(fabs(StackPopFloat())));
			break;
		case NATIVE_METHOD_absI:
			StackPush(nvm_int2stack(abs(StackPopInt())));
			break;
		case NATIVE_METHOD_acos:
			StackPush(VmFloatToStack(acos(StackPopFloat())));
			break;
		case NATIVE_METHOD_asin:
    		StackPush(VmFloatToStack(asin(StackPopFloat())));
			break;
		case NATIVE_METHOD_atan:
			StackPush(VmFloatToStack(atan(StackPopFloat())));
			break;
		case NATIVE_METHOD_atan2:
			a = StackPopFloat();
		    b = StackPopFloat();
    		StackPush(VmFloatToStack(atan2(a,b)));
			break;
		case NATIVE_METHOD_ceil:
			StackPush(VmFloatToStack(ceil(StackPopFloat())));
			break;
		case NATIVE_METHOD_cos:
			StackPush(VmFloatToStack(cos(StackPopFloat())));
			break;
		case NATIVE_METHOD_exp:
			StackPush(VmFloatToStack(exp(StackPopFloat())));
			break;
		case NATIVE_METHOD_floor:
		    StackPush(VmFloatToStack(floor(StackPopFloat())));
			break;
		case NATIVE_METHOD_log:
			StackPush(VmFloatToStack(log(StackPopFloat())));
			break;
		case NATIVE_METHOD_maxF:
			a=StackPopFloat();
    		b=StackPopFloat();
		    StackPush(VmFloatToStack(a>b?a:b));
			break;
		case NATIVE_METHOD_maxI:
			ai=StackPopInt();
    		bi=StackPopInt();
	    	StackPush(nvm_int2stack(ai>bi?ai:bi));
			break;
		case NATIVE_METHOD_minI:
			ai=StackPopInt();
    		bi=StackPopInt();
		    StackPush(nvm_int2stack(ai<bi?ai:bi));
			break;
		case NATIVE_METHOD_minF:
			a=StackPopFloat();
    		b=StackPopFloat();
		    StackPush(VmFloatToStack(a<b?a:b));
			break;
		case NATIVE_METHOD_pow:
			a = StackPopFloat();
    		b = StackPopFloat();
	    	StackPush(VmFloatToStack(pow(a,b)));
			break;
		case NATIVE_METHOD_random:
			StackPush(VmFloatToStack((tVmFloat)rand()/RAND_MAX));
			break;
		case NATIVE_METHOD_rint:
			StackPush(VmFloatToStack((int)(StackPopFloat())));
			break;
		case NATIVE_METHOD_round:
			StackPush(nvm_int2stack((int)(StackPopFloat())));
			break;
		case NATIVE_METHOD_sin:
			StackPush(VmFloatToStack(sin(StackPopFloat())));
			break;
		case NATIVE_METHOD_sqrt:
			StackPush(VmFloatToStack(sqrt(StackPopFloat())));
			break;
		case NATIVE_METHOD_tan:
			StackPush(VmFloatToStack(tan(StackPopFloat())));
			break;
		case NATIVE_METHOD_toDegrees:
			StackPush(VmFloatToStack(180/M_PI*StackPopFloat()));
			break;
		case NATIVE_METHOD_toRadians:
			StackPush(VmFloatToStack(M_PI/180*StackPopFloat()));
			break;
		default:
			HandleError(kErrorNativeUnknownMethod);
			break;

	}
	// TODO: delete this when the switch statement is verified
	/*
  if(mref == NATIVE_METHOD_absF) {
    StackPush(VmFloatToStack(fabs(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_absI) {
    StackPush(nvm_int2stack(abs(StackPopInt())));
  
  } else if(mref == NATIVE_METHOD_acos) {
    StackPush(VmFloatToStack(acos(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_asin) {
    StackPush(VmFloatToStack(asin(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_atan) {
    StackPush(VmFloatToStack(atan(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_atan2) {
    a = StackPopFloat();
    b = StackPopFloat();
    StackPush(VmFloatToStack(atan2(a,b)));
  } else if(mref == NATIVE_METHOD_ceil) {
    StackPush(VmFloatToStack(ceil(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_cos) {
    StackPush(VmFloatToStack(cos(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_exp) {
    StackPush(VmFloatToStack(exp(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_floor) {
    StackPush(VmFloatToStack(floor(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_log) {
    StackPush(VmFloatToStack(log(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_maxF) {
    a=StackPopFloat();
    b=StackPopFloat();
    StackPush(VmFloatToStack(a>b?a:b));
  } else if(mref == NATIVE_METHOD_maxI) {
    ai=StackPopInt();
    bi=StackPopInt();
    StackPush(nvm_int2stack(ai>bi?ai:bi));
  } else if(mref == NATIVE_METHOD_minI) {
    ai=StackPopInt();
    bi=StackPopInt();
    StackPush(nvm_int2stack(ai<bi?ai:bi));
  } else if(mref == NATIVE_METHOD_minF) {
    a=StackPopFloat();
    b=StackPopFloat();
    StackPush(VmFloatToStack(a<b?a:b));
  } else if(mref == NATIVE_METHOD_pow) {
    a = StackPopFloat();
    b = StackPopFloat();
    StackPush(VmFloatToStack(pow(a,b)));
  } else if(mref == NATIVE_METHOD_random) {
    StackPush(VmFloatToStack((tVmFloat)rand()/RAND_MAX));
  } else if(mref == NATIVE_METHOD_rint) {
    StackPush(VmFloatToStack((int)(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_round) {
    StackPush(nvm_int2stack((int)(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_sin) {
    StackPush(VmFloatToStack(sin(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_sqrt) {
    StackPush(VmFloatToStack(sqrt(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_tan) {
    StackPush(VmFloatToStack(tan(StackPopFloat())));
  } else if(mref == NATIVE_METHOD_toDegrees) {
    StackPush(VmFloatToStack(180/M_PI*StackPopFloat()));
  } else if(mref == NATIVE_METHOD_toRadians) {
    StackPush(VmFloatToStack(M_PI/180*StackPopFloat()));
  } else
    HandleError(kErrorNativeUnknownMethod);
	*/
}

#endif //VM_INCLUDE_MATH

