/*============================================================================
  Set, an object-oriented C immutable set class.
  Copyright (C) 2023 by Zack T Smith.

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

#ifndef _OOC_SET_H
#define _OOC_SET_H

#include "Object.h"
#include "Array.h"

#include <stdarg.h>

typedef struct set_list_item {
	Object *object;
	struct set_list_item *next;
} SetListItem;

typedef enum {
	SetSizeSmall = 0, // small number of buckets
	SetSizeMedium = 1, // medium number of buckets
	SetSizeLarge = 2, // large number of buckets
} SetSize;

#define DECLARE_SET_METHODS(TYPE_POINTER) \
	bool (*contains) (TYPE_POINTER, Any*); \
	Array *(*asArray) (TYPE_POINTER); \
	unsigned (*count) (TYPE_POINTER); \
	bool (*isEmpty) (TYPE_POINTER);

// TODO: Union, intersection, subtract.

struct set;

typedef struct setclass {
	DECLARE_OBJECT_CLASS_VARS
	DECLARE_OBJECT_METHODS(struct set*)
	DECLARE_SET_METHODS(struct set*)
} SetClass;

extern SetClass *_SetClass;
extern SetClass* SetClass_init (SetClass*);

#define DECLARE_SET_INSTANCE_VARS(TYPE_POINTER) \
	unsigned nBuckets; \
	SetListItem **buckets;

typedef struct set {
	SetClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct set*)
	DECLARE_SET_INSTANCE_VARS(struct set*)
} Set;

extern Set* Set_init (Set*);
extern Set* Set_newWith (Any*, ...); // RULE: Last item must be NULL.
extern void Set_destroy (Any*);

#endif

