
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "String.h"
#include "MutableString.h"

int main()
{
	String *str;
	MutableString *mut;
	size_t len;
	wchar_t ch;
	Object *obj;

	// Empty string
	str = new(String);
	len = $(str, length);
	if (len != 0)
		return 1;
	release (str);

	// Empty mutable string
	mut = new(MutableString);
	len = $(mut, length);
	if (len != 0)
		return 2;
	release (mut);

	// String from literal C string
	str = _String("this is a test\n"); // 15 chars
	len = $(str, length);
	if (len != 15)
		return 3;

	// Character at String location
	ch = $(str, at, 0);
	if (ch != 't')
		return 4;
	ch = $(str, at, 100);
	if (ch != 0)
		return 5;
	ch = $(str, at, 14);
	if (ch != '\n')
		return 6;
	release(str);

	// Make sure release cleared out the pointer.
	if (str != NULL) {
		return 7;
	}

	// MutableString from literal C string
	mut = MutableString_withCString("this is a test\n"); // 15 chars
	len = $(mut, length);
	if (len != 15)
		return 8;

	String *second = _String("this is a test\n"); 
	if (!$(second, equals, mut)) 
		return 98;
	release(second);

	// Character at MutableString location
	ch = $(mut, at, 0);
	if (ch != 't')
		return 9;
	ch = $(mut, at, 100);
	if (ch != 0)
		return 10;
	ch = $(mut, at, 14);
	if (ch != '\n')
		return 11;

	// Insert character at location
	$(mut, insertCharacterAt, 'A', 0);
	ch = $(mut, at, 0);
	if (ch != 'A')
		return 12;
	ch = $(mut, at, 1);
	if (ch != 't')
		return 13;
	release (mut);

	// Explode test
	String *separator = _String(",");
	Array *ary;
	str = _String("1,2,3");
	ary = $(str, explode, separator);
	size_t count = $(ary, count);
	$(str, print, NULL); printf (" explodes into %lu objects.\n", count);
	if (count != 3) {
		return 14;
	}
	release (ary);
	release (str);

	// Explode test
	str = _String("1,2,");
	ary = $(str, explode, separator);
	count = $(ary, count);
	$(str, print, NULL); printf (" explodes into %lu objects.\n", count);
	if (count != 3) {
		return 15;
	}
	release (ary);
	release (str);

	// Explode test
	str = _String(",2,");
	ary = $(str, explode, separator);
	count = $(ary, count);
	$(str, print, NULL); printf (" explodes into %lu objects.\n", count);
	if (count != 3) {
		return 16;
	}
	release (ary);
	release (str);

	// Explode test
	str = _String(",");
	ary = $(str, explode, separator);
	count = $(ary, count);
	$(str, print, NULL); printf (" explodes into %lu objects.\n", count);
	if (count != 2) {
		return 17;
	}
	release (ary);
	release (str);

	// Explode test
	str = _String(",fubar");
	ary = $(str, explode, separator);
	count = $(ary, count);
	$(str, print, NULL); printf (" explodes into %lu objects.\n", count);
	if (count != 2) {
		return 17;
	}
	release (ary);
	release (str);
	release (separator);

	deallocateClasses();
	return 0;
}

