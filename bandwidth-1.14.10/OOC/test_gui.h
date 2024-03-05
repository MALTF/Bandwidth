/*============================================================================
  TestController, an object-oriented C 3D model controller class.
  Copyright (C) 2024 by Zack T Smith.

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

#ifndef _OOC_TESTCONTROLLER_H
#define _OOC_TESTCONTROLLER_H

#include "Button.h"
#include "Controller.h"

#include <time.h>

#define DECLARE_TESTCONTROLLER_METHODS(TYPE_POINTER) 

struct mycontroller;

typedef struct mycontrollerclass {
	DECLARE_OBJECT_CLASS_VARS
	DECLARE_OBJECT_METHODS(struct mycontroller*)
	DECLARE_CONTROLLER_METHODS(struct mycontroller*)
	DECLARE_TESTCONTROLLER_METHODS(struct mycontroller*)
} TestControllerClass;

extern TestControllerClass *_TestControllerClass;
extern TestControllerClass* TestControllerClass_init (TestControllerClass*);

#define DECLARE_TESTCONTROLLER_INSTANCE_VARS(TYPE_POINTER) \
	Vector lightPositions[2]; \
	Vector cameraPosition; \
	View *view; \
	Label *label; \
	Button *exitButton; \
	time_t t0; \

typedef struct mycontroller {
	TestControllerClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct mycontroller*)
	DECLARE_CONTROLLER_INSTANCE_VARS(struct mycontroller*)
	DECLARE_TESTCONTROLLER_INSTANCE_VARS(struct mycontroller*)
} TestController;

extern TestController* TestController_init (TestController* object);
extern void TestController_destroy (Any *);

#endif

