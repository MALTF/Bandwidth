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

#ifndef _OOC_BUTTON_H
#define _OOC_BUTTON_H

#include "Label.h"

#define DECLARE_BUTTON_METHODS(TYPE_POINTER) 

#define DECLARE_BUTTON_INSTANCE_VARS(TYPE_POINTER) \
	Any *listener;

struct button;

typedef struct buttonclass {
	DECLARE_OBJECT_CLASS_VARS
        DECLARE_OBJECT_METHODS(struct button*)
        DECLARE_VIEW_METHODS(struct button*)
        DECLARE_LABEL_METHODS(struct button*)
        DECLARE_BUTTON_METHODS(struct button*)
} ButtonClass;

extern ButtonClass *_ButtonClass;

typedef struct button {
        ButtonClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct button*)
	DECLARE_VIEW_INSTANCE_VARS(struct button*)
        DECLARE_LABEL_INSTANCE_VARS(struct button*)
	DECLARE_BUTTON_INSTANCE_VARS(struct button*)
} Button;

extern Button *Button_init (Button *self);
extern ButtonClass* ButtonClass_init (ButtonClass*);

#endif

