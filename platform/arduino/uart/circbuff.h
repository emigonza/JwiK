/*
   Circular Buffer

   Creates a 32 byte circular buffer. Contains funcs for putting chars
   and strings in the buffer as well as extracting chars. Buffer size is
   hardcoded for efficiency, since avr has no hardware divider.


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
#ifndef CIRCBUFF_H
#define CIRCBUFF_H

typedef struct {
    char* buf;
    char head;
    char len;
} CircularBuffer;

//returns circular buf struct ptr
CircularBuffer* cb_init(void);
//frees mem alloc'd by cb_init
void cb_free(volatile CircularBuffer *cb);

//puts a char in the buffer
void cb_putc(volatile CircularBuffer *cb, char c);
//puts a string in the buffer
void cb_puts(volatile CircularBuffer *cb, char* s, char len);
//return a char from buffer
char cb_getc(volatile CircularBuffer *cb);
//true if room left in buffer
char cb_hasRoom(volatile CircularBuffer *cb);
//true if data left in buffer
char cb_hasData(volatile CircularBuffer *cb);

void cb_print(volatile CircularBuffer *cb);
#endif //CIRCBUFF_H
