/*============================================================================
  MutableDictionary, an object-oriented C mutable dictionary class.
  Copyright (C) 2023 by Zack T Smith.

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

#ifndef _OOC_MUTABLEDICTIONARY_H
#define _OOC_MUTABLEDICTIONARY_H

#include <stdbool.h>
#include <stdint.h>

#include "Dictionary.h"
#include "String.h"

#define DECLARE_MUTABLEDICTIONARY_METHODS(TYPE_POINTER) \
	void (*setObject) (TYPE_POINTER, String* /* key */, Object* /* value */); \
	void (*remove) (TYPE_POINTER, String* /* key */); 

struct mutabledictionary;

typedef struct mutabledictionaryclass {
	DECLARE_OBJECT_CLASS_VARS
	DECLARE_OBJECT_METHODS(struct mutabledictionary*)
	DECLARE_DICTIONARY_METHODS(struct mutabledictionary*)
	DECLARE_MUTABLEDICTIONARY_METHODS(struct mutabledictionary*)
} MutableDictionaryClass;

extern MutableDictionaryClass *_MutableDictionaryClass;
extern MutableDictionaryClass* MutableDictionaryClass_init (MutableDictionaryClass*);

#define DECLARE_MUTABLEDICTIONARY_INSTANCE_VARS(TYPE_POINTER) 

typedef struct mutabledictionary {
	MutableDictionaryClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct mutabledictionary*)
	DECLARE_DICTIONARY_INSTANCE_VARS(struct mutabledictionary*)
	DECLARE_MUTABLEDICTIONARY_INSTANCE_VARS(struct mutabledictionary*)
} MutableDictionary;

extern MutableDictionary* MutableDictionary_init (MutableDictionary*);
extern MutableDictionary* MutableDictionary_initWithSize (MutableDictionary*, DictionarySize);
extern void MutableDictionary_destroy (Any *);

#endif

