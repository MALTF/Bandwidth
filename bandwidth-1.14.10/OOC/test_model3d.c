
#include <stdio.h>
#include <string.h>

#include "Model3D.h"
#include "Utility.h"

int main()
{
	Model3D *m;

	// Empty model 
	m = new(Model3D);
	if (m->nTriangles)
		return 1;
	if (!m->triangles)
		return 2;
	
	// Model with cube 
	$(m, addSphere, 1.f, 36); // BUG: value of 10 leaves gap.
	$(m, translate, 1.5, 0, 0);
	$(m, addCube, 1.f);
	if (!m->nTriangles)
		return 3;
	if (!m->triangles)
		return 5;

	// Write to file
	$(m, writeSTL, "test.stl");
	if (!file_exists("test.stl"))
		return 5;

	release(m);
	deallocateClasses();

	if (!system ("which fstl")) {
		system ("fstl test.stl");
	}

	//unlink ("test.stl");
	return 0;
}

