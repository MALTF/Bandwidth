/*============================================================================
  Set, an object-oriented C mutable set class.
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

#include "Set.h"
#include "MutableArray.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SetClass *_SetClass = NULL;

void Set_print (Set* self, FILE *file)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Set);

	if (!file) {
		file = stdout;
	}

	unsigned total = $(self, count);

	unsigned count = 0;
	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			$(item->object, print, file);
			if (count != total-1) {
				fprintf (file, ", ");
			} else {
				fprintf (file, "\n");
			}
			item = item->next;
			count++;
		}
	}
}

bool Set_equals (Set* self, void *other_)
{
	if (!self || !other_) {
		return false;
	}
	verifyCorrectClass(self,Set);
	return false;
}

void Set_describe (Set* self, FILE *file)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,Set);
	
	unsigned nObjects = $(self, count);
	fprintf (file ?: stdout, "%s(%u objects, %u buckets)\n", $(self, className), nObjects , (unsigned)self->nBuckets);
}

void Set_destroy (Any* self_)
{
        DEBUG_DESTROY;

	if (!self_)
		return;
	verifyCorrectClassOrSubclass(self_,Set);
	
	Set *self = (Set*) self_;

	if (!self->buckets) {
		return;
	}

	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			SetListItem *next = item->next;
			release (item->object);
			ooc_free (item);
			item = next;
		}
	}
}

unsigned Set_count (Set* self)
{
        DEBUG_DESTROY;

	if (!self) {
		return 0;
	}
	verifyCorrectClassOrSubclass(self,Set);
	
	if (!self->buckets) {
		return 0;
	}

	unsigned count = 0;
	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			count++;
			item = item->next;
		}
	}

	return count;
}

Set* Set_init (Set* self)
{
	ENSURE_CLASS_READY(Set);

	if (self) {
		Object_init ((Object*)self);
		self->is_a = _SetClass;
		self->nBuckets = 127; // SetSizeSmall
		size_t nBytes = sizeof(SetListItem*) * self->nBuckets;
		self->buckets = (SetListItem**) malloc(nBytes);
		ooc_bzero (self->buckets, nBytes);
	}
	return self;
}

Set* Set_newWith (Any *first, ...) // NOTE: Last item must be NULL.
{
	Set *set = new(Set);

	va_list list;
	va_start(list, first);
	Object *object = first;
	do {
		if (isObject(object)) {
			unsigned hash = $(object, hash);
			unsigned index = hash % set->nBuckets;
			SetListItem* item = (SetListItem*) malloc(sizeof(SetListItem));
			item->object = retain(object);
			item->next = NULL;
			set->buckets[index] = item;
		} else {
			Log_error (__FUNCTION__, "Parameter is not an Object.");
		}
		object = va_arg(list, Object*);
	} while (object != NULL);
	va_end(list);

	return set;
}

Array *Set_asArray (Set *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClassOrSubclass(self,Set);

	MutableArray *array = new(MutableArray);
	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			Object *object = item->object;
			$(array, append, object);
			item = item->next;
		}
	}

	Array *result = Array_withArray (array);
	release (array);
	return result;
}

bool Set_contains (Set *self, Any *object_)
{
	if (!self || !object_) {
		return false;
	}
	verifyCorrectClassOrSubclass(self,Set);
	if (!isObject(object_)) {
		return false;
	}

	Object *object = object_;

	unsigned hash = $(object, hash);
	unsigned index = hash % self->nBuckets;

	SetListItem *item = self->buckets[index];
	while (item) {
		// First, check for data match e.g. string equality.
		if ($(object, equals, item->object)) {
			return true;
		}

		// Second, compare pointers.
		if (((void*)object) == (void*) item->object) {
			return true;
		}

		item = item->next;
	}

	return false;
}

bool Set_isEmpty (Set *self)
{
	if (!self) {
		return false;
	}
	verifyCorrectClassOrSubclass(self,Set);

	for (unsigned i=0; i < self->nBuckets; i++) {
		if (self->buckets[i]) {
			return false;
		}
	}
	return true;
}

SetClass* SetClass_init (SetClass *class)
{
	SET_SUPERCLASS(Object);

	SET_OVERRIDDEN_METHOD_POINTER(Set,describe);
        SET_OVERRIDDEN_METHOD_POINTER(Set,destroy);
        SET_OVERRIDDEN_METHOD_POINTER(Set,print);

        SET_METHOD_POINTER(Set,contains);
	SET_METHOD_POINTER(Set,asArray);
	SET_METHOD_POINTER(Set,count);
	SET_METHOD_POINTER(Set,isEmpty);

	VALIDATE_CLASS_STRUCT(class);
	return class;
}

