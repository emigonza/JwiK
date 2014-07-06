//
//  NanoVM, a tiny java VM for the Atmel AVR family
//  Copyright (C) 2005 by Till Harbaum <Till@Harbaum.org>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

//
//  vmstack.h
//

#ifndef VMSTACK_H
#define VMSTACK_H

#include "vm.h"

void StackInitialize(uint8_t staticFields);

// various stack operations
void StackPush(tVmStack val);
tVmStack StackPop(void);
tVmInt StackPopInt(void);
tVmStack StackPeek(uint8_t index);
tVmInt StackPeekInt(uint8_t index);
void   * StackPopAddr(void);
void   * StackPeekAddr(uint8_t index);

# ifdef VM_INCLUDE_FLOAT
tVmFloat StackPopFloat(void);
tVmFloat StackPeekFloat(uint8_t index);
# endif


tVmStack   *GetStackPointer(void);
void StackAddToSp(int8_t offset);

tVmStack StackGetStaticVariable(uint16_t index);
void StackSetStaticVariable(uint16_t index, tVmStack value);

void StackSaveBase(void);
BOOL StackIsEmpty(void);

BOOL StackIsHeapIdInUse(tHeapId id);

#endif // STACK_H
