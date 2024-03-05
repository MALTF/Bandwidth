
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Stack.h"
#include "String.h"
#include "RectStack.h"

int main()
{
	uint32_t value;
	Stack *stack = NULL;

	// Test new, push, pop.
	stack = new(Stack);
	if ($(stack, isEmpty) != true) 
		return 1;
	if ($(stack, pop) != NULL)
		return 2;
	String *str = _String("abc");
	$(stack, push, str);
	if ($(stack, isEmpty) != false) 
		return 3;
	String *str2 = $(stack, pop);
	if (str != str2)
		return 4;
	if ($(stack, isEmpty) != true) 
		return 5;
	release(str);

	// Check order of push/pop.
	str = _String("A");
	str2 = _String("B");
	$(stack, push, str);
	$(stack, push, str2);
	String *s = $(stack, pop);
	if (s != str2 || !$(s, equals, str2))
		return 6;
	s = $(stack, pop);
	if (s != str)
		return 7;

	// Check swap.
	str = _String("x");
	str2 = _String("y");
	$(stack, push, str);
	$(stack, push, str2);
	$(stack, swap);
	s = $(stack, pop);
	if (s != str || !$(s, equals, str))
		return 8;
	s = $(stack, pop);
	if (s != str2 || !$(s, equals, str2))
		return 9;

	// Check pop of empty stack.
	s = $(stack, pop);
	if (s != NULL)
		return 10;
	release(stack);

	RectStack *rects = new(RectStack);
	if ($(rects, isEmpty) != true) 
		return 11;
	Rect r = Rect_new(1,2,3,4);
	$(rects, push, r);
	if ($(rects, isEmpty) == true) 
		return 12;
	Rect r2 = $(rects, pop);
	if ($(rects, isEmpty) != true) 
		return 13;
	if (r2.origin.x != 1 
	  || r2.origin.y != 2
	  || r2.size.width != 3
	  || r2.size.height != 4) {
		return 14;
	}
	release(rects);

	deallocateClasses();
	return 0;
}

