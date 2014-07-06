/*
   Circular Buffer

   Created October 15, 2013
   Copyright (C) 2013 Patrick Schubert

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include "circbuff.h"
#include <util/atomic.h>

#define BUF_SIZE 32
#define OVERFLOW_MASK (BUF_SIZE - 1)

// Creates a Circular Buffer of size BUF_SIZE
CircularBuffer* cb_init(void)
{
    CircularBuffer* cb = malloc(sizeof(CircularBuffer));
    if (cb){
        cb->buf = malloc(sizeof(char)*BUF_SIZE);
        if (!cb->buf){
            free(cb);
            return 0;
        }
        cb->head = 0;
        cb->len = 0;
    }


    return cb;
}


void cb_putc(volatile CircularBuffer *cb, char c)
{
    if (!cb) return;

    if (cb->len < BUF_SIZE){ //if there's room, add char
           ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
           char* tailPtr = cb->buf + ((cb->head + cb->len) & OVERFLOW_MASK);
           *tailPtr = c;  // safely update state
           cb->len++;
       }
    }
    return;
}


void cb_puts(volatile CircularBuffer *cb, char* s, char len)
{
    while (len){
        cb_putc(cb, *s);
        s++;
        len--;
    }
    return ;
}


char cb_getc(volatile CircularBuffer *cb){
    if (!cb) return 0;
    char c = 0;

    if (cb->len){                    // if there's data in buffer
        c = *(cb->buf + cb->head);   // return that data
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            cb->head = (cb->head + 1) & OVERFLOW_MASK; // safely update buffer state
            cb->len -= 1;
        }
    }
    return c;
}

char cb_hasRoom(volatile CircularBuffer *cb){
    return (cb->len < BUF_SIZE);
}

char cb_hasData(volatile CircularBuffer *cb){
    return (cb->len);
}


void cb_free(volatile CircularBuffer *cb)
{
    if (cb){
        free(cb->buf);
        free((void*)cb);
    }
}


void cb_print(volatile CircularBuffer *cb)
{
    if (!cb) return;

    printf("Circular Buff:\n");
    printf("cb->buf:%s\n",cb->buf);
    printf("s:%d,h:%d,l:%d\n\n",BUF_SIZE,cb->head,cb->len);
    return;
}
