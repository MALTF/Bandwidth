
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "String.h"
#include "Array.h"
#include "MutableArray.h"
#include "Int.h"
#include "Double.h"

int main()
{
	Array *ary;
	MutableArray *mut;
	String *str;
	size_t count;
	Object *obj;

	// Empty array
	ary = new(Array);
	count = $(ary, count);
	if (count != 0)
		return 1;
	if (ary->retainCount != 0)
		return 2;
	release (ary);

	// Array with 1 element
	str = String_withCString("abc123");
	ary = Array_withObject((Object*)str);
	count = $(ary, count);
	if (count != 1)
		return 3;
	obj = $(ary, at, 0);
	if (obj != (void*)str)
		return 4;
	obj = $(ary, at, 100);
	if (obj != (Any*)NULL)
		return 5;

	// Bad array index
	obj = $(ary, at, -1);
	if (obj != NULL)
		return 6;
	obj = $(ary, at, 1);
	if (obj != NULL)
		return 7;
	release(ary);
	ary = NULL;

	// Add/remove from mutable array.
	str = String_withCString("xyz");
	mut = new(MutableArray);
	count = $(mut, count);
	if (count != 0)
		return 8;
	$(mut, append, str);
	count = $(mut, count);
	if (count != 1)
		return 9;
	$(mut, append, str);
	count = $(mut, count);
	if (count != 2)
		return 10;
	$(mut, removeAt, 0);
	count = $(mut, count);
	if (count != 1)
		return 11;
	$(mut, removeAt, 0);
	count = $(mut, count);
	if (count != 0)
		return 12;

	// Append 1000 items and verify them
	$(mut, removeAll);
	count = $(mut, count);
	if (count != 0)
		return 13;
	srand (1234);
	for (int i=0; i < 1000; i++)
		$(mut, append, _Int(rand()));
	srand (1234);
	for (int i=0; i < 1000; i++) {
		Int *value = $(mut, at, i);
		if (rand() != $(value, asInt))
			return 14;
	}
	$(mut, removeAll);
	count = $(mut, count);
	if (count != 0)
		return 15;

	$(mut, append, _String("abc"));
	$(mut, append, _Int(123));
	$(mut, append, _Double(1.23));
	if (3 != $(mut, count))
		return 16;
	str = $(mut, at, 0);
	Int *integer = $(mut, at, 1);
	Double *floating = $(mut, at, 2);

	if (0 != strcmp("String", $(str,className)))
		return 17;
	if (0 != strcmp("Int", $(integer,className)))
		return 18;
	if (0 != strcmp("Double", $(floating,className)))
		return 19;
	$(mut, removeAll);
	if (0 != $(mut, count))
		return 20;

	// Empty array
	ary = new(Array);
	if (NULL != $(ary, at, 1000))
		return 21;
	if (NULL != $(ary, at, 1))
		return 22;
	if (NULL != $(ary, at, 0))
		return 23;

	for (int i=0; i < 100; i++) {
		$(mut, insertAt, _Int(i), 0);
	}
	for (int i=0; i < 100; i++) {
		int expectedValue = 99 - i;
		Int *object = $(mut, at, i);
		int value = $(object, asInt);
		if (value != expectedValue) {
			printf ("Found %i at %i, expected %i\n", value,i,expectedValue);
			return 24;
		}
	}
	$(mut, removeAll);

	// Remove random items
	srand (time (NULL));
	for (int run=0; run < 100; run++) {
		for (int i=0; i < 100; i++) {
			$(mut, append, _Int(i));
		}
		unsigned removedValue = rand() % 100;
		// printf ("Removing %u\n", removedValue);
		$(mut, removeAt, removedValue);
		count = $(mut, count);
		for (unsigned i=0; i < count; i++) {
			Int *object = $(mut, at, i);
			int foundValue = $(object, asInt);
			if (foundValue == removedValue)
				return 25;
		}
		$(mut, removeAll);
		if (0 != $(mut, count))
			return 26;
	}

	release(mut);

	if (mut != NULL)
		return 27;

	release(ary);
	if (ary != NULL)
		return 28;

	deallocateClasses();
	return 0;
}

