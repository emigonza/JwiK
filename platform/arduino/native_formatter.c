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
// native_formatter.c
//
// Native Java Format Object and Methods

#include "platform_types.h"
#include "config.h"
#include "vmerror.h"

#ifdef VM_INCLUDE_FORMATTER



#include "native_java.h"

#include "..\..\vmstack.h"
#include "native_formatter.h"
#include "..\..\native_string.h"
//#include "..\..\nvmtypes.h"

#include <math.h>
#include <stdio.h>

#define NATIVE_METHOD_formatI 1
#define NATIVE_METHOD_formatZ 2
#define NATIVE_METHOD_formatF 3

#define FLOAT_DIGS 10
typedef struct 
{
	uint8_t flags;
	uint8_t width;
	uint8_t prec;
	uint8_t conv;
	uint8_t pre_len;
	uint8_t post_len;
	int8_t * post;
} tFormatDescriptor;

// ********************************************************************************************************************
// int16_ternal Functions


// extracts all digits and fill remaining digits with '0'
void IntToStr(int8_t * begin, int8_t * end, uint32_t val, uint8_t base, int8_t a)
{
	while (begin!=end--)
	{
		int8_t c = val%base;
		c += (c>9)?(a-10):'0';
		*end=c;
		val/=base;
	}
}
// format an integer based on the supplied format string
int16_t FormatInt(int8_t * res, tFormatDescriptor * fmtdscr, int16_t val)
{
	int8_t a;
	int8_t c;
	uint8_t base;
	uint8_t prec;
	uint8_t i;
	uint8_t offset;
	int16_t n=0;

	if (val<0)
	{
		*res++='-', n++, val=-val;
	} 
	else if (fmtdscr->flags&0x04) 
	{
		*res++='+', n++;
	} 
	else if (fmtdscr->flags&0x08) 
	{
		*res++=' ', n++;
	}

	a = 'a';
	c = fmtdscr->conv;
	base = 10;
	if (c=='d')
	{
	}
	else if (c=='o')
	{
		base=8;
	}
	else if (c=='x')
	{
		base=16;
	}
	else if (c=='X')
	{
		base=16;
		a='A';
	}

	IntToStr(res, res+10, val, base, a);
	prec = 10;
	i= 0;
	while (prec--)
	{
		if (res[i++]!='0')
	  		break;
	}
	prec++;
	if (fmtdscr->flags&0x10) 
	{
		if (fmtdscr->width-n > prec)
	  		prec = fmtdscr->width-n;
	}
	if (prec==0)
		prec=1;
	offset = 10-prec;
	while(prec--)
	{
		res[0]=res[offset];
		res++, n++;
	}
	return n;
}
// format a bool value based on the supplied format string
int16_t FormatBool(int8_t * res, tFormatDescriptor * fmtdscr, int16_t val)
{
	if (fmtdscr->conv=='b')
  	{
    	if (val)
		{
      		NativeStrCpy(res, "true");
      		return 4;
    	} 
		else 
		{
      		NativeStrCpy(res, "false");
      		return 5;
    	}
  	} 
	else if (fmtdscr->conv=='B')
	{
    	if (val)
		{
      		NativeStrCpy(res, "TRUE");
      		return 4;
    	} 
		else 
		{
			NativeStrCpy(res, "FALSE");
      		return 5;
    	}
  	} 
	else 
	{
    	return 0;
  	}
}
// format a floating point number using the supplied format string
int16_t FormatFloat(int8_t * res, tFormatDescriptor * fmtdscr, float val)
{
  	int16_t n=0;
	int8_t a;
	int8_t conv;
	uint8_t base;
	int8_t max_prec;
	int8_t prec;
	float factA, factB;
	int8_t pre;
	uint8_t offs;
	uint8_t x;
	uint8_t i;
	int8_t i1;
	int8_t exp;

  	if (val<0)
	{
    	*res++='-', n++, val=-val;
  	} 
	else if (fmtdscr->flags&0x04) 
	{
    	*res++='+', n++;
  	} else if (fmtdscr->flags&0x08) 
	{
    	*res++=' ', n++;
  	}
  
  	a = 'a';
  	conv = fmtdscr->conv;
  	base = 10;
  	if (conv=='a')
	{
    	base=16;
  	}
	else if (conv=='A')
	{
    	base=16;
    	a='A';
  	}

  	max_prec = 8;
  	prec = max_prec;

  	factA = 1.0;
  	for (i=0; i<prec-1; ++i)
	{
    	factA*=(float)base;
  	}
  	factB = base*factA;
  
  	exp=0;

  	if (val>0.0) 
	{
    	exp=prec-1;
    	while(val>=factB)
      		val/=base, exp+=1;
    	while(val<factA)
      	val*=base, exp-=1;
  	}
  	IntToStr(res, res+prec, val, base, a);

  	while (prec>0)
	{
    	if (res[prec-1]!='0')
      		break;
    	prec--;
  	}
  	//prec contains number of significant digits

	// wenn exp>=0 exp+1 = Ziffern vor dem Komma
  	// wenn exp<0  |abs| = Nullen vor erster Ziffer
  	if ((exp<-4) || (exp>prec)) 
	{
    	if (conv=='g') conv='e';
    	if (conv=='G') conv='E';
  	} 
	else 
	{
    	if (conv=='g') conv='f';
    	if (conv=='G') conv='f';
  	}
  
  	pre=1;
  	if (conv=='f')
	{
    	if (exp>=0) 
		{
      		pre += exp;
    	}
		else 
		{
      		pre=0;
    	}
  	}
  	if (prec>pre)
    	prec -= pre;
  	else
    	prec=0;
  
  	if (fmtdscr->prec != 255)
    	prec = fmtdscr->prec;
  	if (fmtdscr->flags&0x02)
	{
    	if (prec==0)
      		prec=1;
  	}

  	offs = 1;
  	if (pre==0)
	{
    	offs = -exp + 1;
    	prec += exp + 1;
    	if (prec<0) 
		{
      		prec=0;
    	}
  	}
  	for (i1=pre+prec; i1>=pre; i1--)	
	{
    	if (i>=max_prec)
      		res[i1+offs]='0';
    	else
      		res[i1+offs]=res[i1];
  	}
  
  	if (pre==0) 
	{
    	*res++='0', n++;
    	if (prec>0)
		{
      		*res++='.', n++;
      		while(++exp<0)
        		*res++='0', n++;
    	} 
		else 
		{
      		x = fmtdscr->prec;
      		if ((x != 255)&&(x!=0)) 
			{
        		*res++='.', n++;
       	 		while(x--)
          			*res++='0', n++;
      		}
    	}
  	} 
	else if (prec>0) 
	{
    	res[pre]='.';
    	res++, n++;
  	} 
	else 
	{
  	}

  	res+=pre+prec, n+=pre+prec;

  	if (conv!='f')
	{
    	*res++=conv, n++;
    	if (exp<0)
		{
      		*res++='-', n++;
      		exp=-exp;
    	} 
		else 
		{
      		*res++='+', n++;
    	}
    	IntToStr(res, res+2, exp, base, a);
    	n+=2;
  	}
  
  	return n;
}
// create a new format descriptor
void CreateFormatDescriptor(tFormatDescriptor * fmtdscr, int8_t * fmt)
{
	uint8_t mode=0;
	int8_t c;

	fmtdscr->flags = 0;
	fmtdscr->width = 0;
	fmtdscr->prec = 255;
	fmtdscr->conv = 0;
	fmtdscr->pre_len = 0;
	fmtdscr->post_len = 0;
	fmtdscr->post = 0;

	// TODO: clean this up and get rid of the goto statements
	while((c=NativeGetChar(fmt++)))
	{
    	switch (mode)
		{
      		case 0:
        		if (c=='%') mode=1;
        		else fmtdscr->pre_len++;
        		break;

      		case 1:
        		if      (c=='-') fmtdscr->flags|=0x01;
        		else if (c=='#') fmtdscr->flags|=0x02;
        		else if (c=='+') fmtdscr->flags|=0x04;
        		else if (c==' ') fmtdscr->flags|=0x08;
        		else if (c=='0') fmtdscr->flags|=0x10;
        		else if (c=='.') fmtdscr->prec=0, mode=3;
        		else goto label2;
        		break;

      		label2:
      		case 2:
        		mode=2;
        		if ((c>='0') && (c<='9')) 
				{
          			fmtdscr->width*=10;
          			fmtdscr->width+=c-'0';
        		} 
				else if (c=='.')
				{
          			fmtdscr->prec=0, mode=3;
        		} 
				else 
				{
          			goto label4;
        		}
				break;
      		case 3:
        		if ((c>='0') && (c<='9')) 
				{
          			fmtdscr->prec*=10;
          			fmtdscr->prec+=c-'0';
        		} 
				else 
				{
          			goto label4;
        		}
        		break;

      		label4:
      		case 4:
        		mode=4;
        		fmtdscr->conv=c;
        		fmtdscr->post=fmt;
        		mode=5;
        		break;
        
      		case 5:
        		fmtdscr->post_len++;
    }
  }
}
// ***************************************************************************************************************
// Public API

// void NativeFormatInvoke(uint8_t methodReference)
//
// Invoke a method on the Formatter object
//
// Args:
//  uint8_t methodReference - reference of method to invoke
// Returns:
//  Nothing
void NativeFormatInvoke(uint8_t methodReference) 
{
 	int8_t res[50];
	int16_t len;
	uint8_t add;
	int16_t memoffset;
	tHeapId id;
	int8_t * dst;
 	tFormatDescriptor fmtdscr;
  	int8_t * fmt = StackPeekAddr(0);

	CreateFormatDescriptor(&fmtdscr, fmt);

  	len = 0;
	switch(methodReference)
	{
		case NATIVE_METHOD_FORMATI:
			//tVmByte val = StackPeek_int16_t(1);
	    	len = FormatInt(res, &fmtdscr, StackPeekInt(1));
			break;
		case NATIVE_METHOD_FORMATZ:
			//tVmInt val = StackPeek_int16_t(1);
    		len = FormatBool(res, &fmtdscr, StackPeekInt(1));
			break;
		case NATIVE_METHOD_FORMATF:
			//tVmFloat val = StackPeekFloat(1);
			len = FormatFloat(res, &fmtdscr, StackPeekFloat(1));
			break;
		default:
			HandleError(kErrorNativeUnknownMethod);
			break;
	}
	// TODO: get rid of this code when the switch statement is verified
	/*
  	if(methodReference == NATIVE_METHOD_FORMATI) 
	{
    nvm_int16_t_t val = StackPeek_int16_t(1);
    len = FormatInt(res, &fmtdscr, val);
  } else if(methodReference == NATIVE_METHOD_FORMATZ) {
    nvm_int16_t_t val = StackPeek_int16_t(1);
    len = FormatBool(res, &fmtdscr, val);
  } else if(methodReference == NATIVE_METHOD_FORMATF) {
    nvm_float_t val = StackPeekFloat(1);
    len = FormatFloat(res, &fmtdscr, val);
  } else
    error(ERROR_NATIVE_UNKNOWN_METHOD);
*/    

  	add = 0;
  	if (fmtdscr.width>len)
    	add = (fmtdscr.width-len);
    
  	// allocate heap and realign strings (address may be changed by gc...)
  	id = HeapAlloc(FALSE, len + add + fmtdscr.pre_len + fmtdscr.post_len + 1);
  	memoffset = (int8_t*)StackPeekAddr(0)-(int8_t*)fmt;
  	fmt+=memoffset;
  	fmtdscr.post+=memoffset;
  	dst = HeapGetAddress(id);
  
  	// build result string
  	NativeStrPartialCpy(dst, fmt, fmtdscr.pre_len);
  	dst+=fmtdscr.pre_len;

  	if (!(fmtdscr.flags&0x01))
	{
    	while(add--)
      	*dst++=' ';
  	}

  	dst+=len;
  
  	if (fmtdscr.flags&0x01)
	{
    	while(add--)
      	*dst++=' ';
  	}
	NativeStrPartialCpy(dst, fmtdscr.post, fmtdscr.post_len);
	dst+=fmtdscr.post_len;
	*dst=0;
	StackPop();
	StackPop();
	StackPush(VM_TYPE_HEAP | id);
}

#endif //VM_INCLUDE_MATH

