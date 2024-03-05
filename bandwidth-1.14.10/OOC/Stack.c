// (C) Zack Smith 2018

#include "Stack.h"
#include "Log.h"

StackClass *_StackClass = NULL;

void Stack_destroy (Any *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	MutableArray_destroy (self);
}

Stack *Stack_init (Stack *self)
{
	ENSURE_CLASS_READY(Stack);
	if (self) {
		MutableArray_init ((MutableArray*) self);
		self->is_a = _StackClass;
	}
	return self;
}

static void Stack_push (Stack *self, Any *item)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	if (!item) 
		Log_errorNullParameter  (__FUNCTION__);

	$(self, append, item);
}

// NOTE! This always returns an object that has been retained.
static Any *Stack_pop (Stack *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	Object *last = $(self, last);
	if (last) {
		retain(last);
		$(self, removeLast);
	}
	return last;
}

static Any *Stack_tos (Stack *self)
{
	if (!self) {
		return NULL;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	return $(self, last);
}

static bool Stack_isEmpty (Stack *self)
{
	if (!self) {
		return true;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	return 0 == $(self, count);
}

static void Stack_swap (Stack *self)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Stack);

	unsigned count = $(self, count);
	if (count >= 2) {
		Any *last = self->array[count-1];
		Any *nextToLast = self->array[count-2];
		self->array[count-1] = nextToLast;
		self->array[count-2] = last;
	}
}

StackClass *StackClass_init (StackClass* class)
{
        SET_SUPERCLASS(MutableArray);

	SET_OVERRIDDEN_METHOD_POINTER(Stack,destroy);

	SET_METHOD_POINTER(Stack,isEmpty);
	SET_METHOD_POINTER(Stack,push);
	SET_METHOD_POINTER(Stack,pop);
	SET_METHOD_POINTER(Stack,tos);
	SET_METHOD_POINTER(Stack,swap);

        VALIDATE_CLASS_STRUCT(class);
	return class;
}

