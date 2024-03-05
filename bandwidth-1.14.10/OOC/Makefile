#============================================================================
# Object Oriented C, an object-oriented programming facility for C.
# Copyright (C) by Zack T Smith.
# This is covered by the Lesser GNU Public License version 3.
#============================================================================

HAVE_CONFIG := $(shell test -e ./Makefile-config && echo yes)
ifeq (${HAVE_CONFIG},yes)
    $(info Found Makefile-config)
    include Makefile-config
else
    # If the user wants to clean, let him clean.
    # Otherwise tell him to configure.
    ifneq (clean,$(MAKECMDGOALS))
        $(error You must run ./configure first.)
    endif
endif

CFLAGS= -Wno-conditional-type-mismatch -I/usr/include/freetype2 
CC=gcc -Wall 
SRC=main.c 
OBJ=main.o
OOC_SRC= ObjectOriented.c Object.c Image.c Image_shrink.c MutableImage.c Double.c Int.c Array.c MutableArray.c SimpleGraphing.c String.c MutableString.c Font.c Console.c DateTime.c FontBuiltin.c FontPK.c cmr5.c cmr6.c cmr9.c cmr12.c Log.c Data.c MutableData.c Utility.c GraphicsTypes.c Stack.c RectStack.c OperatingSystem.c Hardware.c CPU.c Dictionary.c MutableDictionary.c Set.c MutableSet.c Model3D.c URL.c FontFreetype.c List.c Display.c Window.c View.c Controller.c Label.c Button.c
OOC_OBJ= ObjectOriented.o Object.o Image.o Image_shrink.o MutableImage.o Double.o Int.o Array.o MutableArray.o SimpleGraphing.o String.o MutableString.o Font.o Console.o DateTime.o FontBuiltin.o FontPK.o cmr5.o cmr6.o cmr9.o cmr12.o Log.o Data.o MutableData.o Utility.o GraphicsTypes.o Stack.o RectStack.o OperatingSystem.o Hardware.o CPU.o Dictionary.o MutableDictionary.o Set.o MutableSet.o Model3D.o URL.o FontFreetype.o List.o Display.o Window.o View.o Controller.o Label.o Button.o
ASM_OBJ=
AS=nasm -f elf64
GL=-lGL -lglut -lGLU

UNAMEMACHINE := $(shell uname -m)
UNAMESYS := $(shell uname -s)
ISCYGWIN := $(shell uname -a | grep Cygwin | sed "s/^.*Cygwin/Cygwin/" | sed "s/Cygwin.*/Cygwin/" )
IS32BIT := $(shell file /bin/ls | grep 32-bit | sed "s/^.*32-bit/32-bit/" | sed "s/32-bit.*/32-bit/" )

ifeq (${UNAMESYS},Linux)
	ifeq (${UNAMEMACHINE},i386)
		AS=nasm -f elf
		ASM_SRC+=utility-x86-32bit.asm
		ASM_OBJ+=utility-x86-32bit.o
	else ifeq (${UNAMEMACHINE},i486)
		AS=nasm -f elf
		ASM_SRC+=utility-x86-32bit.asm
		ASM_OBJ+=utility-x86-32bit.o
	else ifeq (${UNAMEMACHINE},i686)
		AS=nasm -f elf
		ASM_SRC+=utility-x86-32bit.asm
		ASM_OBJ+=utility-x86-32bit.o
	else ifeq (${UNAMEMACHINE},x86_64)
		# RPi OS for x86 has 32-bit userspace and 64-bit kernel, necessitating 32-bit check.
		ifeq (${IS32BIT},32-bit)
			ASM_SRC+=utility-x86-32bit.asm
			ASM_OBJ+=utility-x86-32bit.o
		else
			ASM_SRC+=utility-x86-64bit.asm
			ASM_OBJ+=utility-x86-64bit.o
		endif
	endif
endif
ifeq (${UNAMESYS},Darwin)
	ifeq (${UNAMEMACHINE},x86_64)
		ASM_SRC+=utility-x86-64bit.asm
		ASM_OBJ+=utility-x86-64bit.o
		AS=nasm -f macho64
	endif
	GL=-framework OpenGL
endif
ifeq (${ISCYGWIN},Cygwin)
        AS=nasm -f win64                            
        ASM_SRC+=utility-x86-64bit.asm
        ASM_OBJ+=utility-x86-64bit.o
endif

libOOC.a: ${OOC_SRC} ${ASM_OBJ} 
	${CC} ${CFLAGS} -c ${OOC_SRC} 
	ar rvs libOOC.a ${OOC_OBJ} ${ASM_OBJ}

LIBS:=-lm

utility-x86-64bit.o:
	${AS} utility-x86-64bit.asm -o utility-x86-64bit.o

utility-x86-32bit.o:
	${AS} utility-x86-32bit.asm -o utility-x86-32bit.o

CONFIG=Makefile-config
ifeq ($(shell test -e $(CONFIG) && echo -n yes),yes)
    $(info $(CONFIG) exists.)
endif

tests:	libOOC.a test_arrays test_strings test_images test_vectors test_stacks test_mprotect 

test_arrays:	test_arrays.c libOOC.a
	@gcc test_arrays.c -o test_arrays libOOC.a ${LIBS}
	@./test_arrays && echo PASSED array test 

test_strings:	test_strings.c libOOC.a
	@gcc test_strings.c -o test_strings libOOC.a ${LIBS}
	@./test_strings && echo PASSED string test 

test_images:	test_images.c libOOC.a
	@gcc test_images.c -o test_images libOOC.a ${LIBS}
	@./test_images && echo PASSED image test

test_stacks:	test_stacks.c libOOC.a
	@gcc test_stacks.c -o test_stacks libOOC.a ${LIBS}
	@./test_stacks && echo PASSED stack test 

test_dictionary:	test_dictionary.c libOOC.a
	@gcc test_dictionary.c -o test_dictionary libOOC.a ${LIBS}
	@./test_dictionary && echo PASSED dictionary test 

test_sets:	test_sets.c libOOC.a
	@gcc test_sets.c -o test_sets libOOC.a ${LIBS}
	@./test_sets && echo PASSED set test 

test_sorting:	test_sorting.c libOOC.a
	@gcc test_sorting.c -o test_sorting libOOC.a ${LIBS}
	@./test_sorting && echo PASSED sorting test 

test_mprotect:	test_mprotect.c libOOC.a
	@gcc test_mprotect.c -o test_mprotect libOOC.a ${LIBS}
	@./test_mprotect && echo PASSED test of vtable protection

test_url:	test_url.c libOOC.a
	@gcc test_url.c -o test_url libOOC.a ${LIBS} -lcurl
	@./test_url && echo PASSED URL test 

test_model3d:	test_model3d.c libOOC.a
	@gcc -g test_model3d.c -o test_model3d libOOC.a ${LIBS} 
	@./test_model3d && echo PASSED Model3D test 

test_list:	test_list.c libOOC.a
	@gcc -g test_list.c -o test_list libOOC.a ${LIBS} 
	@./test_list && echo PASSED List test 

test_gui:	test_gui.c libOOC.a
	@gcc -g test_gui.c -o test_gui libOOC.a ${LIBS} ${GL}
	@./test_gui && echo PASSED GUI test 

clean:
	chmod 644 *.c *.h *.txt Makefile* *.asm # *.tif *.jpg
	chmod 755 configure
	rm -f Makefile-config config.h 
	rm -f ${OOC_OBJ} libOOC.a OOC32.a OOC64.a
	rm -f utility-x86-32bit.o utility-x86-64bit.o
	rm -f test_stacks test_url test_arrays test_strings test_images 
	rm -f test_vectors test_gui test_mprotect
	rm -f test_sets test_dictionary test_model3d test_list
