#ifndef _OOC_STACK_H
#define _OOC_STACK_H

// (C) Zack Smith 2018
// Stack implements a stack of Objects.

#include "MutableArray.h"

#define DECLARE_STACK_INSTANCE_VARS(TYPE_POINTER) 

#define DECLARE_STACK_METHODS(TYPE_POINTER) \
	bool (*isEmpty) (TYPE_POINTER); \
	void (*push) (TYPE_POINTER, Any *item); \
	Any* (*pop) (TYPE_POINTER); \
	Any* (*tos) (TYPE_POINTER); \
	void (*swap) (TYPE_POINTER); 

struct stack;

typedef struct stackclass {
	DECLARE_OBJECT_CLASS_VARS
        DECLARE_OBJECT_METHODS(struct stack*)
        DECLARE_ARRAY_METHODS(struct stack*)
        DECLARE_MUTABLE_ARRAY_METHODS(struct stack*)
        DECLARE_STACK_METHODS(struct stack*)
} StackClass;

extern StackClass *_StackClass;
extern StackClass *StackClass_init (StackClass*);

typedef struct stack {
        StackClass *is_a;
	DECLARE_OBJECT_INSTANCE_VARS(struct stack*)
	DECLARE_ARRAY_INSTANCE_VARS(struct stack*)
	DECLARE_MUTABLE_ARRAY_INSTANCE_VARS(struct stack*)
	DECLARE_STACK_INSTANCE_VARS(struct stack*)
} Stack;

extern Stack *Stack_new ();
extern Stack *Stack_init (Stack*);
extern void Stack_destroy (Any *);

#endif
