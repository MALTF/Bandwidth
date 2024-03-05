/*============================================================================
  URL, an object-oriented URL class.
  Copyright (C) 2018 by Zack T Smith.

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

#ifndef _URL_H
#define _URL_H

#include "Object.h"
#include "String.h"
#include "Data.h"
#include "MutableString.h"

//#include <math.h>

typedef enum { 
	URL_SUCCESS = 0,
	URL_BAD_PARAMETER = -1,
	URL_BAD_SCHEME = -2,
	URL_BAD_HOST = -3,
	URL_CANNOT_CONNECT = -4,
	URL_CANNOT_SEND = -5,
	URL_SSL_NOT_SUPPORTED = -6,
	URL_FAILED_REDIRECT = -7,
	URL_CANNOT_RECEIVE = -8,
} URLError;

typedef enum {
	HTTPOperationNone = 0,
	HTTPOperationGet,
	HTTPOperationPost,
	HTTPOperationPut,
	HTTPOperationDelete,
} HTTPOperation;

typedef enum {
	URLSchemeNone = 0,
	URLSchemeHTTP,
	URLSchemeHTTPS,
	URLSchemeFTP,
	URLSchemeMailto,
	URLSchemeTelephone,
	URLSchemeIRC,
	URLSchemeSMS,
	URLSchemeSSH,
	URLSchemeFile,
	URLSchemeJavascript,
	URLSchemeGIT,
} URLScheme;

#define DECLARE_URL_INSTANCE_VARS(TYPE_POINTER)  \
	char *url_string; \
	uint8_t *read_buffer; \
	unsigned long read_buffer_index; \
	unsigned long read_buffer_size; \
	URLScheme scheme; \
	HTTPOperation operation;

#define DECLARE_URL_METHODS(TYPE_POINTER) \
	String *(*string) (TYPE_POINTER); \
	void (*setCString) (TYPE_POINTER, const char* url); \
	void (*setHTTPOperation) (TYPE_POINTER, HTTPOperation); \
	HTTPOperation (*httpOperation) (TYPE_POINTER); \
	URLScheme (*scheme) (TYPE_POINTER); \
	Data* (*synchronousGet) (TYPE_POINTER, unsigned *statusReturn); \
	Data* (*synchronousPost) (TYPE_POINTER, uint8_t *postData, size_t postLength, unsigned *statusReturn); 

//String* (*synchronousWait) (TYPE_POINTER, unsigned port, uint32_t *ipAddressReturn, int *errorReturn);

struct url;

typedef struct urlclass {
	DECLARE_OBJECT_CLASS_VARS
        DECLARE_OBJECT_METHODS(struct url*)
        DECLARE_URL_METHODS(struct url*)
} URLClass;

extern URLClass *_URLClass;
extern URLClass *URLClass_init (URLClass*);

typedef struct url {
        URLClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct url*)
	DECLARE_URL_INSTANCE_VARS(struct url*)
} URL;

extern URL *URL_new (void);
extern URL *URL_init (URL *self);
extern URL *URL_withCString (const char* url);
#define _URL(VALUE) URL_withCString(VALUE)

#endif
