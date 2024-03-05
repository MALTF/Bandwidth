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

#include "List.h"
#include "String.h"
#include "Int.h"
#include "Double.h"

#include <string.h>

ListClass *_ListClass = NULL;

static ListItem*
ListItem_new ()
{
	ListItem *item = (ListItem*) malloc(sizeof(ListItem));
	if (!item) {
		return NULL;
	}
	memset (item, 0, sizeof(ListItem));
	return item;
}

static void
ListItem_destroy (ListItem *item)
{
	if (!item) {
		return;
	}
	if (item->object) {
		release (item->object);
	}
	free (item);
}

static void List_removeAll (List *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	ListItem *item = self->first;
	while (item) {
		ListItem *next = item->next;
		ListItem_destroy (item);
		item = next;
	}

	self->first = NULL;
	self->last = NULL;
	self->iterator = NULL;
}

static void List_destroy (Any *self)
{
        DEBUG_DESTROY;

	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	List_removeAll (self);
}

static void List_describe (List* self, FILE *outputFile) 
{ 
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	if (!outputFile) {
		outputFile = stdout;
	}

	fprintf (outputFile, "%s", $(self, className));
}

static void List_append (List *self, Any *object_)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);
	if (!isObject(object_)) {
		return;
	}

	Object *object = object_;

	ListItem *item = ListItem_new ();
	item->object = retain(object);

	if (!self->last) {
		self->first = item;
	} else {
		self->last->next = item;
	}
	self->last = item;
}

static void List_appendCString (List *self, const char* string) {
	List_append (self, _String(string ?: ""));
}
static void List_appendInt (List *self, int value) {
	List_append (self, _Int(value));
}
static void List_appendDouble (List *self, double value) {
	List_append (self, _Double(value));
}

static void List_prepend (List *self, Any *object_)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);
	if (!isObject(object_)) {
		return;
	}

	Object *object = object_;

	ListItem *item = ListItem_new ();
	item->object = retain(object);
	item->next = self->first;
	self->first = item;
}

static Any *List_first (List *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClass(self,List);

	if (!self->first) {
		return NULL;
	}
	return self->first->object;
}

static Any *List_last (List *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClass(self,List);

	if (!self->last) {
		return NULL;
	}
	return self->last->object;
}

static void List_removeFirst (List *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	if (!self->first) {
		return;
	}

	ListItem *item = self->first;
	self->first = item->next;
	ListItem_destroy (item); // This will release object.
}

static void List_removeLast (List *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	if (!self->last) {
		return;
	}

	ListItem *item = self->last;

	if (self->first == item) {
		ListItem_destroy (item); 
		self->first = NULL;
		self->last = NULL;
		return;
	}

	ListItem *previous = self->first;
	while (previous && previous->next != self->last) {
		previous = previous->next;
	}

	if (!previous) {
		// Internal error
		return;
	}

	self->last = previous;
	previous->next = NULL;
	ListItem_destroy (item); 
}

static Any *List_at (List *self, unsigned position)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClass(self,List);

	ListItem *item = self->first;
	while (position > 0 && item) {
		item = item->next;
		position--;
	}
	if (!item || position > 0) {
		return NULL;
	}
	return item->object;
}

static void List_insertAt (List *self, Any *object_, unsigned position)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);
	if (!isObject(object_)) {
		return;
	}

	if (!position) {
		List_prepend (self, object_);
		return;
	}

	Object *object = object_;

	ListItem *previous = self->first;
	ListItem *next = previous->next;
	while (position > 0 && previous && next) {
		previous = previous->next;
		next = next->next;
		position--;
	}

	if (!previous) {
		return;
	}

	ListItem *item = ListItem_new();
	item->object = retain(object);
	previous->next = item;
	item->next = next;
}

void List_print (List *self, FILE *output)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	if (!output) {
		output = stdout;
	}

	ListItem *item = self->first;
	while (item) {
		$(item->object, print, output);
		ListItem *next = item->next;
		if (next) {
			fputc (',', output);
		}
		item = next;
	}
}

static void List_removeAt (List *self, unsigned position)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	if (!self->first) {
		return;
	}

	if (!position) {
		List_removeFirst (self);
		return;
	}

	ListItem *previous = self->first;
	ListItem *item = previous->next;
	position--;

	while (position > 0 && previous && item) {
		previous = previous->next;
		item = item->next;
		position--;
	}

	if (!previous || position > 0) {
		return;
	}

	previous->next = item->next;
	item->next = NULL;
	ListItem_destroy(item);
}

static bool List_isEmpty (List *self)
{
	if (!self) {
		return false;
	}
	verifyCorrectClass(self,List);

	return NULL == self->first;
}

static void List_iterate (List *self) 
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,List);

	self->iterator = self->first;
}

static Any *List_next (List *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClass(self,List);

	ListItem *item = self->iterator;
	if (!item) {
		return NULL;
	}

	Object *object = item->object;
	self->iterator = item->next;

	return object;
}

static size_t List_count (List *self)
{
	if (!self) {
		return 0LU;
	}
	verifyCorrectClass(self,List);

	ListItem *item = self->first;
	size_t count = 0LU;
	while (item) {
		count++;
		item = item->next;
	}

	return count;
}

List* List_init (List *self)
{
	ENSURE_CLASS_READY(List);
	if (!self) {
		return NULL;
	}

	Object_init ((Object*) self);

	self->is_a = _ListClass;
	self->first = NULL;
	self->last = NULL;
	self->iterator = NULL;

	return self;
}

ListClass* ListClass_init (ListClass *class)
{
	SET_SUPERCLASS(Object);

	SET_OVERRIDDEN_METHOD_POINTER(List,describe);
	SET_OVERRIDDEN_METHOD_POINTER(List,destroy);
	SET_OVERRIDDEN_METHOD_POINTER(List,print);

	SET_METHOD_POINTER(List,removeAll);
	SET_METHOD_POINTER(List,isEmpty);
	SET_METHOD_POINTER(List,append);
	SET_METHOD_POINTER(List,prepend);
	SET_METHOD_POINTER(List,appendCString);
	SET_METHOD_POINTER(List,appendInt);
	SET_METHOD_POINTER(List,appendDouble);
	SET_METHOD_POINTER(List,first);
	SET_METHOD_POINTER(List,last);
	SET_METHOD_POINTER(List,removeFirst);
	SET_METHOD_POINTER(List,removeLast);
	SET_METHOD_POINTER(List,removeAt);
	SET_METHOD_POINTER(List,insertAt);
	SET_METHOD_POINTER(List,at);
	SET_METHOD_POINTER(List,iterate);
	SET_METHOD_POINTER(List,next);
	SET_METHOD_POINTER(List,count);
	
	VALIDATE_CLASS_STRUCT(class);
	return class;
}

