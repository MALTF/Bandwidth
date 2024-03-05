/*============================================================================
  List, an object-oriented C linked list class.
  Copyright (C) 2009, 2010, 2021 by Zack T Smith.

  Object-Oriented C is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  Object-Oriented C is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public License
  along with this software.  If not, see <http://www.gnu.org/licenses/>.

  The author may be reached at 1@zsmith.co.
 *===========================================================================*/

#ifndef _OOC_LIST_H
#define _OOC_LIST_H

#include <stdbool.h>
#include <math.h>

#include "Object.h"

typedef struct listitem {
	Object *object;
	struct listitem *next;
} ListItem;

extern ListItem* ListItem_new_string (char *str);

#define DECLARE_LIST_INSTANCE_VARS(FOO) \
	ListItem *first, *last; \
	ListItem *iterator;

#define DECLARE_LIST_METHODS(TYPE_POINTER) \
	void (*append) (TYPE_POINTER, Any*); \
	void (*appendCString) (TYPE_POINTER, const char*); \
	void (*appendInt) (TYPE_POINTER, int); \
	void (*appendDouble) (TYPE_POINTER, double); \
	void (*prepend) (TYPE_POINTER, Any*); \
	Any *(*first) (TYPE_POINTER); \
	Any *(*last) (TYPE_POINTER); \
	Any *(*at) (TYPE_POINTER, unsigned position); \
	void (*removeFirst) (TYPE_POINTER); \
	void (*removeLast) (TYPE_POINTER); \
	void (*removeAt) (TYPE_POINTER, unsigned position); \
	void (*insertAt) (TYPE_POINTER, Any *, unsigned position); \
	void (*removeAll) (TYPE_POINTER); \
	bool (*isEmpty) (TYPE_POINTER); \
	void (*iterate) (TYPE_POINTER); \
	Any *(*next) (TYPE_POINTER); \
	size_t (*count) (TYPE_POINTER); 

struct linkedlist;

typedef struct linkedlistclass {
	DECLARE_OBJECT_CLASS_VARS
        DECLARE_OBJECT_METHODS(struct linkedlist*)
        DECLARE_LIST_METHODS(struct linkedlist*)
} ListClass;

extern ListClass *_ListClass;

typedef struct linkedlist {
        ListClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct linkedlist*)
	DECLARE_LIST_INSTANCE_VARS(struct linkedlist*)
} List;

extern List *List_init (List *self);
extern ListClass* ListClass_init (ListClass*);

#endif

