/*============================================================================
  MutableSet, an object-oriented C mutable set class.
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

#ifndef _OOC_MUTABLESET_H
#define _OOC_MUTABLESET_H

#include <stdbool.h>
#include <stdint.h>

#include "Set.h"

#define DECLARE_MUTABLESET_METHODS(TYPE_POINTER) \
	void (*add) (TYPE_POINTER, Any*);\
	void (*remove) (TYPE_POINTER, Any*);\
	void (*removeAll) (TYPE_POINTER);\
	void (*unionWith) (TYPE_POINTER, Set*);\
	void (*subtract) (TYPE_POINTER, Set*);\
	void (*intersect) (TYPE_POINTER, Set*);

struct mutableset;

typedef struct mutablesetclass {
	DECLARE_OBJECT_CLASS_VARS
	DECLARE_OBJECT_METHODS(struct mutableset*)
	DECLARE_SET_METHODS(struct mutableset*)
	DECLARE_MUTABLESET_METHODS(struct mutableset*)
} MutableSetClass;

extern MutableSetClass *_MutableSetClass;
extern MutableSetClass* MutableSetClass_init (MutableSetClass*);

#define DECLARE_MUTABLESET_INSTANCE_VARS(TYPE_POINTER) 

typedef struct mutableset {
	MutableSetClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct mutableset*)
	DECLARE_SET_INSTANCE_VARS(struct mutableset*)
	DECLARE_MUTABLESET_INSTANCE_VARS(struct mutableset*)
} MutableSet;

extern MutableSet* MutableSet_init (MutableSet*);
extern MutableSet* MutableSet_initWithSize (MutableSet*, SetSize);
extern void MutableSet_destroy (Any *);

#endif

