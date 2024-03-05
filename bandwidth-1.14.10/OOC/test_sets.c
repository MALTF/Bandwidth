
#include <stdio.h>

#include "MutableSet.h"
#include "String.h"

int main()
{
	Set *set;
	MutableSet *mut;
	unsigned total;

	// Empty mutable set
	mut = new(MutableSet);
	total = $(mut, count);
	if (total != 0)
		return 1;
	if ($(mut, isEmpty) != true) 
		return 2;
	release (mut);

	String *object1 = String_withCString("fromage");

	// 1 item set
	mut = new(MutableSet);
	$(mut, add, object1);
	total = $(mut, count);
	if (total != 1)
		return 2;
	if ($(mut, isEmpty) != false) 
		return 3;
	if (!$(mut, contains, String_withCString("fromage")))
		return 4;
	if ($(mut, contains, String_withCString("cheese")))
		return 5;
	$(mut, add, String_withCString("Kaese"));
	$(mut, add, String_withCString("kaas"));
	if (!$(mut, contains, String_withCString("Kaese")))
		return 6;
	if (!$(mut, contains, String_withCString("kaas")))
		return 7;
	$(mut, print, NULL);

	$(mut, remove, object1);
	total = $(mut, count);
	if (total > 2)
		return 7;

	$(mut, remove, String_withCString("Kaese"));
	total = $(mut, count);
	if (total > 1)
		return 10;

	$(mut, remove, String_withCString("kaas"));
	total = $(mut, count);
	if (total > 0)
		return 11;

	release (mut);

	//----------
	// Union
	mut = new(MutableSet);
	$(mut, add, _String("cheese"));
	$(mut, add, _String("fromage"));
	printf ("set 1: ");
	$(mut, print, NULL);
	if ($(mut, count) != 2) 
		return 12;

	set = Set_newWith(_String("formaggio"), NULL);
	printf ("set 2: ");
	$(set, print, NULL);

	$(mut, unionWith, set);
	printf ("union of 1 & 2: ");
	$(mut, print, NULL);
	if ($(mut, count) != 3) 
		return 13;

	//-------------
	// Intersection
	printf ("set 1: ");
	$(mut, print, NULL);
	printf ("set 2: ");
	$(set, print, NULL);
	$(mut, intersect, set);
	printf ("intersection of 1 & 2: ");
	$(mut, print, NULL);
	if ($(mut, count) != 1) 
		return 20;

	//-------------
	// Subtraction
	$(mut, add, _String("cheese"));
	$(mut, add, _String("fromage"));
	$(mut, add, _String("formaggio"));
	printf ("set 1: ");
	$(mut, print, NULL);
	printf ("set 2: ");
	$(set, print, NULL);
	$(mut, subtract, set);
	printf ("subtract 2 from 1: ");
	$(mut, print, NULL);
	if ($(mut, count) != 2) 
		return 21;

	// Remove all
	$(mut, removeAll);
	if ($(mut, count) != 0) 
		return 22;
	if ($(mut, isEmpty) != true) 
		return 23;

	//----------
	// Empty set
	set = new(Set);
	total = $(set, count);
	if (total != 0)
		return 30;
	if ($(set, isEmpty) != true) 
		return 31;
	release (set);

	//-----------
	// 1-item set
	String *object4 = String_withCString("formaggio");
	set = Set_newWith(object4, NULL);
	total = $(set, count);
	if (total != 1)
		return 41;
	if (! $(set, contains, object4))
		return 42;
	$(set, print, NULL);
	release (set);

	deallocateClasses();
	return 0;
}

