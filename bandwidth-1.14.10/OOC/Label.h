/*============================================================================
  Label, an object-oriented C label class.
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

#ifndef _OOC_LABEL_H
#define _OOC_LABEL_H

#include "Object.h"
#include "View.h"

#define DECLARE_LABEL_METHODS(TYPE_POINTER) \
	void (*setString) (TYPE_POINTER, Any*); \
	void (*setCString) (TYPE_POINTER, const char*); 

#define DECLARE_LABEL_INSTANCE_VARS(TYPE_POINTER) \
	String *string; \
	Font *font;

struct label;

typedef struct labelclass {
	DECLARE_OBJECT_CLASS_VARS
        DECLARE_OBJECT_METHODS(struct label*)
        DECLARE_VIEW_METHODS(struct label*)
        DECLARE_LABEL_METHODS(struct label*)
} LabelClass;

extern LabelClass *_LabelClass;

typedef struct label {
        LabelClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct label*)
	DECLARE_VIEW_INSTANCE_VARS(struct label*)
	DECLARE_LABEL_INSTANCE_VARS(struct label*)
} Label;

extern Label *Label_init (Label *self);
extern void Label_destroy (Any *self);
extern LabelClass* LabelClass_init (LabelClass*);

#endif

