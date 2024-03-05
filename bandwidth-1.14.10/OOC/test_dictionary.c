
#include <stdio.h>
#include <time.h>

#include "MutableDictionary.h"

int main()
{
	Dictionary *dict;
	MutableDictionary *mut;
	unsigned total;

	// Empty mutable dictionary
	mut = new(MutableDictionary);
	total = $(mut, count);
	if (total != 0)
		return 1;
	release (mut);

	String *key1 = String_withCString("ok");
	String *key2 = String_withCString("something");
	String *value1 = String_withCString("what");
	retain(key1);
	retain(key2);
	retain(value1);

	// 1 item dictionary
	mut = new(MutableDictionary);
	$(mut, setObject, key1, (Object*) value1);
	total = $(mut, count);
	if (total != 1)
		return 1;
	Object *value = $(mut, object, String_withCString("ok"));
	if (value == NULL)
		return 2;
	if (!$(((String*)value), equals, value1))
		return 3;
	value = $(mut, object, String_withCString("ok2"));
	if (value != NULL)
		return 4;
	$(mut, setObject, key2, (Object*) String_withCString("else"));
	$(mut, print, NULL);

	$(mut, remove, key1);
	total = $(mut, count);
	if (total > 1)
		return 5;

	$(mut, remove, key2);
	total = $(mut, count);
	if (total > 0)
		return 5;

	release (mut);

	// Empty dictionary
	dict = new(Dictionary);
	total = $(dict, count);
	if (total != 0)
		return 6;
	release (dict);

	// 1-item dictionary
	String *key3 = String_withCString("hey");
	dict = Dictionary_newWith(key3, (Object*) value1);
	total = $(dict, count);
	if (total != 1)
		return 7;
	value = $(dict, object, key3);
	if (value == NULL)
		return 8;
	if (!$(value, equals, value1))
		return 9;
	$(dict, print, NULL);
	release (dict);

	release(key1);
	release(key2);
	release(value1);

	deallocateClasses();
	return 0;
}

