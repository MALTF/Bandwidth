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

#include "MutableSet.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MutableSetClass *_MutableSetClass = NULL;

MutableSet* MutableSet_initWithSize (MutableSet* self, SetSize size)
{
	ENSURE_CLASS_READY(MutableSet);

	if (self) {
		Object_init ((Object*)self);

		// Skip call to Set_init since that doesn't do anything 
		// more than this, and it will allocate buckets.

		self->is_a = _MutableSetClass;
		switch (size) {
		default:
		case SetSizeSmall:
			self->nBuckets = 127;
			break;
		case SetSizeMedium:
			self->nBuckets = 1279;
			break;
		case SetSizeLarge:
			self->nBuckets = 12721;
			break;
		}
		size_t nBytes = sizeof(SetListItem*) * self->nBuckets;
		self->buckets = (SetListItem**) malloc(nBytes);
		ooc_bzero (self->buckets, nBytes);
	}
	return self;
}

MutableSet* MutableSet_init (MutableSet* self)
{
	ENSURE_CLASS_READY(MutableSet);

	if (self) {
		Set_init ((Set*)self);
		self->is_a = _MutableSetClass;
	}

	return self;
}

void MutableSet_add (MutableSet *self, Any *object_)
{
	if (!self || !object_) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);
	if (!isObject(object_)) {
		return;
	}
	Object *object = object_;

	unsigned hash = $(object, hash);
	unsigned index = hash % self->nBuckets;

	SetListItem *item = self->buckets[index];
	bool found = false;
	while (item) {
		if ($(object, equals, item->object)) {
			found = true;
			break;
		}
		item = item->next;
	}
	if (!found) {
		SetListItem *item = (SetListItem*) malloc(sizeof(SetListItem));
		item->object = retain(object);
		item->next = self->buckets[index];
		self->buckets[index] = item;
	}
}

void MutableSet_remove (MutableSet *self, Any *object_)
{
	if (!self || !object_) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);
	if (!isObject(object_)) {
		return;
	}
	Object *object = object_;

	unsigned hash = $(object, hash);
	unsigned index = hash % self->nBuckets;

	SetListItem *item = self->buckets[index];
	SetListItem *prev = NULL;
	while (item) {
		if (areSameClass (object, item->object)) {
			if ($(object, equals, item->object)) {
				if (!prev) {
					self->buckets[index] = item->next;
				} else {
					prev->next = item->next;
				}
				release(item->object);
				free (item);
				break;
			}
		}
		prev = item;
		item = item->next;
	}
}

void MutableSet_unionWith (MutableSet *self, Set *set_)
{
	if (!self || !set_) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);
	verifyCorrectClassOrSubclass(set_,Set);
	Set *set = set_;

	for (unsigned i=0; i < set->nBuckets; i++) {
		SetListItem *item = set->buckets[i];
		while (item) {
			MutableSet_add (self, item->object);
			item = item->next;
		}
	}
}

void MutableSet_subtract (MutableSet *self, Set *set_)
{
	if (!self || !set_) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);
	verifyCorrectClassOrSubclass(set_,Set);
	Set *set = set_;

	for (unsigned i=0; i < set->nBuckets; i++) {
		SetListItem *item = set->buckets[i];
		while (item) {
			MutableSet_remove (self, item->object);
			item = item->next;
		}
	}
}

/*---------------------------------------------------------------------------
 * Name:        intersect
 * Purpose:     Obtains the intersection of sets, changing self.
 *-------------------------------------------------------------------------*/
void MutableSet_intersect (MutableSet *self, Set *set_)
{
	if (!self || !set_) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);
	verifyCorrectClassOrSubclass(set_,Set);
	Set *set = set_;

	SetListItem *prev = NULL;
	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			Object *object = item->object;
			SetListItem *next = item->next;

			// If any of our objects in NOT the other set, remove it.
			if (!$(set, contains, object)) {
				if (!prev) {
					self->buckets[i] = next;
				} else {
					prev->next = next;
				}
				release(item->object);
				free (item);
			} else {
				prev = item;
			}
			item = next;
		}
	}
}

void MutableSet_removeAll (MutableSet *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,MutableSet);

	for (unsigned i=0; i < self->nBuckets; i++) {
		SetListItem *item = self->buckets[i];
		while (item) {
			SetListItem *next = item->next;
			release(item->object);
			free(item);
			item = next;
		}
		self->buckets[i] = NULL;
	}
}

MutableSetClass* MutableSetClass_init (MutableSetClass *class)
{
	SET_SUPERCLASS(Set);

        SET_METHOD_POINTER(MutableSet,add);
        SET_METHOD_POINTER(MutableSet,remove);
        SET_METHOD_POINTER(MutableSet,unionWith);
        SET_METHOD_POINTER(MutableSet,subtract);
        SET_METHOD_POINTER(MutableSet,intersect);
        SET_METHOD_POINTER(MutableSet,removeAll);

	VALIDATE_CLASS_STRUCT(class);
	return class;
}

