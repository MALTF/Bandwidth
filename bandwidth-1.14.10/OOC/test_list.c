
#include <stdio.h>
#include <math.h>

#include "List.h"
#include "String.h"
#include "Int.h"
#include "Double.h"

int main()
{
	List *list = new(List);
	if (!$(list, isEmpty))
		return 1;

	$(list, appendCString, "string");
	if ($(list, isEmpty))
		return 2;

	$(list, appendInt, -10001);
	$(list, appendDouble, M_PI);

	if ($(list, count) != 3) 
		return 3;

	Double *d = $(list, at, 2);
	if (d) {
		printf ("element 2 = ");
		$(d, print, NULL);
		puts ("");
	} else {
		return 4;
	}

	printf ("list=");
	$(list, print, NULL);
	puts ("");

	$(list, iterate);
	Object *object;
	while ((object = $(list, next))) {
		printf ("FOUND ITEM ");
		$(object, print, NULL);
		puts ("");
	}

	$(list, removeFirst);
	if ($(list, count) != 2) 
		return 5;
	$(list, removeLast);
	if ($(list, count) != 1) 
		return 6;
	$(list, removeAll);
	if ($(list, count) != 0) 
		return 7;

	$(list, appendInt, 555);
	if ($(list, count) != 1) 
		return 8;

	object = $(list, at, 1000);
	if (object != NULL)
		return 9;

	$(list, appendInt, 111);
	if ($(list, count) != 2) 
		return 10;

	$(list, prepend, _Int(2));
	if ($(list, count) != 3) 
		return 11;

	printf ("list=");
	$(list, print, NULL);
	puts ("");

	$(list, removeAt, 1);
	if ($(list, count) != 2) 
		return 12;

	$(list, removeAt, 1000); // no effect
	if ($(list, count) != 2) 
		return 13;

	printf ("list=");
	$(list, print, NULL);
	puts ("");

	$(list, removeAt, 1);

	deallocateClasses();
	return 0;
}

