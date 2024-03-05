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

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Display.h"
#include "Window.h"
#include "Controller.h"
#include "View.h"
#include "Model3D.h"
#include "FontPK.h"

#include "test_gui.h"
#include "Log.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>

#define CUSTOM_VIEW_SIZE 180
#define MARGIN 10

TestControllerClass *_TestControllerClass = NULL;

#define kDefaultTestControllerSize (32)

void TestController_destroy (Any* self)
{
        DEBUG_DESTROY;

	if (!self) {
		return;
	}
	verifyCorrectClass(self,TestController);
	Controller_destroy (self);
}

TestController* TestController_init (TestController* self)
{
	ENSURE_CLASS_READY(TestController);

	if (!self) {
		return NULL;
	}
	
	Controller_init ((Controller*)self);
	self->is_a = _TestControllerClass;

	self->lightPositions[0] = Vector_new(0,0,0);
	self->lightPositions[1] = Vector_new(4,0,0);
	return self;
}

bool TestController_construct (TestController *self)
{
	if (!self) { 
		return false;
	}
	verifyCorrectClass(self,TestController);
	Window *window = self->window;
	if (!window) {
		return false;
	}

	self->t0 = time(NULL);

	int width = window->width;
	int height = window->height;

	Model3D *m;
	m = new(Model3D);
	m->color = RGB_CYAN;
	// m->color = RGB_TEST_PATTERN;
	$(m, addCone, 1.f, 1.f, 30);
	$(m, translate, 1, 0, 0);
	$(m, addCube, 1.f);
	$(m, translate, 1, 0, 0);
	$(m, addSphere, 1.f, 360);
	$(m, translate, 1, 0, 0);
	$(m, addCylinder, 1.f, 1.f, 360);
	$(m, translate, -2, 0, 0);
	$(window, addModel3D, m);

	$(window, setLightEnabled, 0, true);
	$(window, setLightColors, 0, RGB_CLEAR, 0x808080, 0x808080);
	//$(window, setLightPosition, 0, Vector_new(10, 10, 0));

	$(window, setLightEnabled, 1, true);
	$(window, setLightColors, 1, 0x202020, RGB_RED, RGB_CLEAR);

	$(window, setLightEnabled, 2, true);
	$(window, setLightColors, 1, 0x202020, RGB_GREEN, RGB_CLEAR);

	$(window, setLightEnabled, 3, true);
	$(window, setLightColors, 1, 0x202020, RGB_BLUE, RGB_CLEAR);

	self->cameraPosition = Vector_new(0,0,3.5);
	$(window, moveCamera, self->cameraPosition);

	View *view = self->view = retain(new(View));
	$(view, defineBorder, ViewBorderTypeRounded, RGB_YELLOW, 1, 30);
	$(window, addView, view);

	self->label = retain(new(Label));
	$(self->label, setBackgroundColor, RGB_RED);
	$(self->label, setForegroundColor, RGB_YELLOW);
	$(window, addView, self->label);

	self->exitButton = retain(new(Button));
	$(self->exitButton, setCString, "Exit Button");
	$(window, addView, self->exitButton);
	self->exitButton->listener = self;

	return true;
}

static long TestController_message (TestController* self, long message, Any *sender, long first, long second)
{
	if (message == kClickedMessage) {
		exit (0);
	}
	return 0;
}

static void TestController_layout (TestController* self)
{
	if (!self) {
		return;
	}
	verifyCorrectClass(self,TestController);
	if (!self->window || !self->label) {
		return;
	}
	
	unsigned width = $(self->window, width);
	unsigned height = $(self->window, height);
	int labelWidth = 200;
	int labelHeight = 44;
	int buttonWidth = 200;
	int buttonHeight = 44;
	const int size = CUSTOM_VIEW_SIZE;
	$(self->label, setRect, Rect_new(MARGIN, height - labelHeight - MARGIN, labelWidth, labelHeight));
	$(self->view, setRect, Rect_new(width - MARGIN - size, MARGIN, size, size));
	$(self->exitButton, setRect, Rect_new(width - buttonWidth - MARGIN, 
						height - buttonHeight - MARGIN, 
						buttonWidth, buttonHeight));
}

bool TestController_update (TestController *self)
{
	if (!self) { 
		return false;
	}
	verifyCorrectClass(self,TestController);

	Window *window = self->window;
	if (!window) {
		return false;
	}

	static int hideCounter = 0;
	hideCounter = (hideCounter + 1) % 200;
	$(self->view, setHidden, hideCounter < 150 ? false : true);

	float x, y, z;

	$(window, setLightPosition, 0, self->lightPositions[0]);
	$(window, setLightPosition, 1, Vector_new(10, 7, 0));
	$(window, setLightPosition, 2, Vector_new(9, 8, 0));
	$(window, setLightPosition, 3, Vector_new(9.5, 7.5, 0));

	// Move the camera.
	static float camera_xdelta = 0.05;
	static float camera_ydelta = 0.01;
	static float camera_zdelta = 0.1;
	x = self->cameraPosition.x;
	y = self->cameraPosition.y;
	z = self->cameraPosition.z;
	x += camera_xdelta;
	y += camera_ydelta;
	z -= camera_zdelta;
	if (fabs(z) > 10) {
		camera_xdelta *= -1;
		camera_ydelta *= -1;
		camera_zdelta *= -1;
	}
	self->cameraPosition = Vector_new(x, y, z);
	$(window, moveCamera, self->cameraPosition);

#ifdef DEBUG
	printf ("Camera location: %s\n", Vector_toString(self->cameraPosition));
#endif

	View *view = self->view;
	$(view->image, fillWithColor, RGB_PURPLE);
	$(view, drawBorder);
	Font *font = (Font*) FontPK_with ("cmr", 18);
	const int size = CUSTOM_VIEW_SIZE;
	$(view->image, drawCString, "CUSTOM VIEW", 10, size/2 - 9, font, RGB_WHITE);
	$(view->image, drawLine, 0, 0, size-1, size-1, RGB_ORANGE);
	$(view->image, drawCircle, size/2,size/2, 70, 0xff, RGB_YELLOW);
	release(font);

	time_t now = time(NULL);
	time_t diff = now - self->t0;
	char timestring[32];
	snprintf (timestring, sizeof(timestring), "Label %02lu:%02lu", diff / 60, diff % 60);
	$(self->label, setCString, timestring);

	return true;
}

static void TestController_dismantle (TestController *self)
{
	if (!self) { 
		return;
	}
	verifyCorrectClass(self,TestController);

	puts(__FUNCTION__);
}

TestControllerClass* TestControllerClass_init (TestControllerClass *class)
{
	SET_SUPERCLASS(Controller);

	// Overridden method
	SET_OVERRIDDEN_METHOD_POINTER(TestController,construct);
	SET_OVERRIDDEN_METHOD_POINTER(TestController,update);
	SET_OVERRIDDEN_METHOD_POINTER(TestController,layout);
	SET_OVERRIDDEN_METHOD_POINTER(TestController,dismantle);
	SET_OVERRIDDEN_METHOD_POINTER(TestController,message);

	VALIDATE_CLASS_STRUCT(class);
	return class;
}

int main()
{
	Display *dpy = new(Display);

	const int windowWidth = 800;
	const int windowHeight = 600;

	Log_show_debug_output = true;

	Window *window = Window_newWith (dpy, "Test", windowWidth, windowHeight, 100, 160);
	TestController *controller = new(TestController);
	$(window, setController, controller);

	$(dpy, mainLoop);

	release(dpy);
	release(window);
	release(controller);

	deallocateClasses();
	return 0;
}

