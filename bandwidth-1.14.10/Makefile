#============================================================================
# bandwidth, a benchmark to estimate memory transfer bandwidth.
# Copyright (C) 2005-2024 by Zack T Smith.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# The author may be reached at 1 at zsmith dot co.
#============================================================================

# Note: 32-bit is no longer supported by Mac OS.

UNAMESYS := $(shell uname -s)
UNAMEPROC := $(shell uname -p)
UNAMEMACHINE := $(shell uname -m)

ISCYGWIN := $(shell uname -a | grep Cygwin | sed "s/^.*Cygwin/Cygwin/" | sed "s/Cygwin.*/Cygwin/" )
IS32BIT := $(shell file /bin/ls | grep 32-bit | sed "s/^.*32-bit/32-bit/" | sed "s/32-bit.*/32-bit/" )

LIB=-lm -L./OOC -lOOC

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

ifeq (${UNAMESYS},Darwin)
	ifeq (${UNAMEMACHINE},arm64)
		include Makefile-darwin-arm64
	else ifeq (${UNAMEMACHINE},arm)
		include Makefile-darwin-arm64
	else
		include Makefile-darwin-x86_64
	endif
else ifeq (${UNAMESYS},Linux)
	ifeq (${UNAMEMACHINE},armv7l)
		include Makefile-linux-arm32
	else ifeq (${UNAMEMACHINE},aarch64)
		ifeq (${IS32BIT},32-bit)
			include Makefile-linux-arm32
		else
			include Makefile-linux-arm64
		endif
	else ifeq (${UNAMEMACHINE},i386)
		include Makefile-linux-i386
	else ifeq (${UNAMEMACHINE},i486)
		include Makefile-linux-i386
	else ifeq (${UNAMEMACHINE},i686)
		include Makefile-linux-i386
	else ifeq (${UNAMEMACHINE},x86_64)
		ifeq (${IS32BIT},32-bit)
			include Makefile-linux-i386
		else
			include Makefile-linux-x86_64
		endif
	else ifeq (${UNAMEMACHINE},riscv64)
		include Makefile-linux-riscv64
	endif
else ifeq (${UNAMESYS},Windows)
	include Makefile-windows
else ifeq (${ISCYGWIN},Cygwin)
	include Makefile-windows
endif

message:
	@echo "To compile for 32-bit i386 Linux:     make -f Makefile-linux-i386"
	@echo "To compile for 64-bit x86_64 Linux:   make -f Makefile-linux-x86_64"
	@echo "To compile for 32-bit ARM Linux:      make -f Makefile-linux-arm32"
	@echo "To compile for 64-bit ARM Linux:      make -f Makefile-linux-arm64"
	@echo "To compile for 64-bit RISC-V Linux:   make -f Makefile-riscv64"
	@echo "To compile for 64-bit ARM Mac OS/X:   make -f Makefile-darwin-arm64"
	@echo "To compile for 64-bit x86 Mac OS/X:   make -f Makefile-darwin-x86_64"
	@echo "To compile for 64-bit Windows+Cygwin: make -f Makefile-windows bandwidth64"
	@echo "To compile for 32-bit Windows+Cygwin: make -f Makefile-windows bandwidth32"

configure:
	@echo "Run ./configure first."
	@exit

clean:
	chmod 644 *.c *.asm *.h *.txt Makefile* output/*
	rm -f main.o bandwidth bandwidth32 bandwidth64 
	rm -f Testing.o TestingARM.o TestingX86.o TestingRISCV.o 
	rm -f CPUInfo.o CPUInfoX86.o CPUInfoARM.o CPUInfoRISCV.o
	rm -f bandwidth32.exe bandwidth64.exe
	rm -f utility-x86-32bit.o utility-x86-64bit.o
	rm -f bandwidth.bmp bandwidth.tif bandwidth32 bandwidth64 
	rm -f routines-x86-32bit.o routines-x86-64bit.o 
	rm -f routines-arm-32bit.o routines-arm-64bit.o routines-riscv-64bit.o
	rm -rf bandwidth64.dSYM bandwidth32.dSYM
	rm -f Makefile-config config.h
	( cd OOC; make clean )

.PHONY:	clean
