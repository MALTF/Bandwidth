
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "String.h"
#include "MutableArray.h"

int main()
{
	MutableArray *mut;

	mut = new(MutableArray);
	$(mut, append, _String("lorem");
	$(mut, append, _String("ipsum");
	$(mut, append, _String("dolor");
	$(mut, append, _String("sit");
	$(mut, append, _String("amet");
	$(mut, append, _String("consectetur");
	$(mut, append, _String("adipiscing");
	$(mut, append, _String("elit");
	$(mut, append, _String("sed");
	$(mut, append, _String("do");
	$(mut, append, _String("eiusmod");
	$(mut, append, _String("tempor");
	$(mut, append, _String("incididunt");
	$(mut, append, _String("ut");
	$(mut, append, _String("labore");
	$(mut, append, _String("et");
	$(mut, append, _String("dolore");
	$(mut, append, _String("magna");
	$(mut, append, _String("aliqua");
		
	printf ("Before quicksort: ");
	$(mut, print, NULL);

	$(mut, quicksort);

	printf ("After quicksort: ");
	$(mut, print, NULL);

	deallocateClasses();
	return 0;
}

