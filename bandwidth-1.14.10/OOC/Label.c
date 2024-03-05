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

#include "Label.h"
#include "String.h"
#include "FontPK.h"

LabelClass *_LabelClass = NULL;

static void Label_setString (Label *self, Any *string_)
{
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Label);
	verifyCorrectClassOrSubclass(string_,String);
	String *string = string_;

	if (self->string) {
		release(self->string);
		self->string = NULL;
	}

	self->string = retain(string);
	self->needsRedraw = true;
}

static void Label_setCString (Label *self, const char *str)
{
	if (!self) {
		return;
	}

	Label_setString (self, str ? _String(str) : NULL);
}

void Label_destroy (Any *self_)
{
        DEBUG_DESTROY;

	if (!self_) {
		return;
	}
	verifyCorrectClass(self_,Label);
	Label *self = self_;

	if (self->string) {
		release(self->string);
		self->string = NULL;
	}

	View_destroy(self_);
}

static void Label_describe (Label* self, FILE *outputFile) 
{ 
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Label);

	if (!outputFile) {
		outputFile = stdout;
	}

	fprintf (outputFile, "%s", $(self, className));
}

static void Label_redraw (Label* self)
{ 
	if (!self) {
		return;
	}
	verifyCorrectClassOrSubclass(self,Label);

	if (!self->needsRedraw) {
		return;
	}

	$(self, clear);

	MutableImage *image = self->image;
	if (!image) {
		return;
	}

	String *string = self->string;
	if (!string) {
		return;
	}

	bool allocatedFont = false;
	Font *font = self->font;
	if (!font) {
		allocatedFont = true;
		font = (Font*) FontPK_with ("cmr", 26);
	}

	int w = self->rect.size.width;
	int h = self->rect.size.height;

	int width = 0, ascent = 0, descent = 0;
	$(font, sizeOfString, string, &width, &ascent, &descent);
	int x = (w - width) / 2;
	int y = (h - (ascent + descent)) / 2;
	$(image, drawString, self->string, x, y, font, self->foregroundColor);

	if (allocatedFont) {
		release(font);
	}

	self->needsRedraw = false;
}

Label* Label_init (Label *self)
{
	ENSURE_CLASS_READY(Label);
	if (!self) {
		return NULL;
	}

	View_init ((View*) self);

	self->is_a = _LabelClass;
	self->string = NULL;

	return self;
}

LabelClass* LabelClass_init (LabelClass *class)
{
	SET_SUPERCLASS(View);

	SET_OVERRIDDEN_METHOD_POINTER(Label,describe);
	SET_OVERRIDDEN_METHOD_POINTER(Label,destroy);
	SET_OVERRIDDEN_METHOD_POINTER(Label,redraw);

	SET_METHOD_POINTER(Label,setString);
	SET_METHOD_POINTER(Label,setCString);
	
	VALIDATE_CLASS_STRUCT(class);
	return class;
}

