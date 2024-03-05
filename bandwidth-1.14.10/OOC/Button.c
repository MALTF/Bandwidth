/*============================================================================
  Button, an object-oriented C button class.
  Copyright (C) 2018, 2019, 2024 by Zack T Smith.

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

// This is derived from my FrugalWidgets project.

#include "Button.h"
#include "String.h"
#include "FontPK.h"

ButtonClass *_ButtonClass = NULL;

static void Button_destroy (Any *self)
{
        DEBUG_DESTROY;

	if (!self) {
		return;
	}
	verifyCorrectClass(self,Button);

	Label_destroy(self);
}

static long Button_message (Button* self, long message, Any *sender, long first, long second)
{
        DEBUG_DESTROY;

	if (!self) {
		return 0;
	}
	verifyCorrectClass(self,Button);

	switch (message) {
		case kClickedMessage: 
			if (self->listener) {
				Object *recipient = self->listener;
				return $(recipient, message, message, self, 0, 0);
			}
			break;
		default:
			break;
	}

	return 0;
}

Button* Button_init (Button *self)
{
	ENSURE_CLASS_READY(Button);
	if (!self) {
		return NULL;
	}

	Label_init ((Label*) self);

	self->is_a = _ButtonClass;
	self->string = NULL;

	return self;
}

ButtonClass* ButtonClass_init (ButtonClass *class)
{
	SET_SUPERCLASS(Label);

	SET_OVERRIDDEN_METHOD_POINTER(Button,destroy);
	SET_OVERRIDDEN_METHOD_POINTER(Button,message);

	VALIDATE_CLASS_STRUCT(class);
	return class;
}

